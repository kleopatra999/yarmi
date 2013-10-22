
#ifndef _yarmi__server_hpp
#define _yarmi__server_hpp

#include "session_base.hpp"

#include <boost/noncopyable.hpp>

#include <cstdint>

#include <string>
#include <sstream>
#include <unordered_set>
#include <functional>

namespace yarmi {

/***************************************************************************/

template<typename UC, template<typename> class GC>
struct server: private boost::noncopyable {
private:
	static bool default_on_connected_predicate(const boost::asio::ip::tcp::endpoint &) {
		return true;
	}
	static void default_error_handler(const std::string &msg) {
		std::cerr << "YARMI: server: \"" << msg << "\"" << std::endl << std::flush;
	}

public:
	template<
		 typename CP = decltype(&default_on_connected_predicate)
		,typename EH = decltype(&default_error_handler)
	>
	server(
		 const std::string &ip
		,std::uint16_t port
		,boost::asio::io_service &ios
		,GC<UC> &gc
		,CP cp = &default_on_connected_predicate
		,EH eh = &default_error_handler
	)
		:acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port))
		,gc(gc)
		,cp(cp)
		,eh(eh)
	{}

	void start() {
		auto session = session_base::create<UC>(
			 acceptor.get_io_service()
			,gc
			,[this](session_base *session) {
				if ( ! gc.has_session(session) ) {
					std::ostringstream os;
					os << "YARMI: session " << std::hex << session << " not in connected sessions list";
					eh(os.str());
				} else {
					gc.del_session(session);
				}

				session->set_on_destruction(true);

				try {
					session->on_disconnected();
				} catch (const std::exception &ex) {
					std::ostringstream os;
					os << "YARMI: [exception] session->on_disconnected(): \"" << ex.what() << "\"";
					eh(os.str());
				}

				try {
					delete session;
				} catch (const std::exception &ex) {
					std::ostringstream os;
					os << "YARMI: [exception] delete session: \"" << ex.what() << "\"";
					eh(os.str());
				}
			}
		);

		acceptor.async_accept(
			 session->get_socket()
			,[this, session](const boost::system::error_code &ec) {
				if ( ! ec ) {
					const boost::asio::ip::tcp::endpoint &ep = session->get_socket().remote_endpoint();
					if ( ! cp(ep) ) {
						std::ostringstream os;
						os << "YARMI: IP \"" << ep.address().to_string() << "\" is in backlist";
						eh(os.str());
					} else {
						gc.add_session(session.get());

						try {
							session->on_connected();
						} catch (const std::exception &ex) {
							std::ostringstream os;
							os << "YARMI: [exception] session->on_connected(): \"" << ex.what() << "\"";
							eh(os.str());
						}

						/** start session */
						session->start();
					}

					/** start accepting next connection */
					start();
				}
			}
		);
	}

private:
	boost::asio::ip::tcp::acceptor acceptor;
	GC<UC> &gc;

	std::function<bool(const boost::asio::ip::tcp::endpoint &)> cp;
	std::function<void(const std::string &)> eh;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__server_hpp
