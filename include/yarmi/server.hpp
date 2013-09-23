#ifndef _yarmi__server_hpp
#define _yarmi__server_hpp

#include "session_base.hpp"

#include <boost/noncopyable.hpp>

#include <string>
#include <unordered_set>
#include <cstdint>
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
		std::cout << "server: \"" << msg << "\"" << std::endl;
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
		,CP cp = default_on_connected_predicate
		,EH eh = default_error_handler
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
					std::cerr << "session 0x" << std::hex << session << " not in connected sessions list" << std::endl << std::flush;
				}
				try {
					session->on_disconnected();
				} catch (const std::exception &ex) {
					std::cerr << "[exception] session->on_disconnected(): \"" << ex.what() << "\"" << std::endl << std::flush;
				}
				gc.del_session(session);
				delete session;
			}
		);

		acceptor.async_accept(
			 session->get_socket()
			,[this, session](const boost::system::error_code &ec) {
				if ( ! ec ) {
					gc.add_session(session.get());
					try {
						session->on_connected();
					} catch (const std::exception &ex) {
						std::cerr << "[exception] session->on_connected(): \"" << ex.what() << "\"" << std::endl << std::flush;
					}
					session->start();
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
