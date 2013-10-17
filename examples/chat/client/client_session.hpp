
#ifndef _yarmi__client_session_hpp
#define _yarmi__client_session_hpp

#include <protocol.hpp>
#include <yarmi/client.hpp>

#include <boost/noncopyable.hpp>

/***************************************************************************/

struct client_session: yarmi::client_base<client_session>, yarmi::client_invoker<client_session> {
	client_session(boost::asio::io_service &ios);

	void on_registration(const std::string &msg, const std::string &regkey);
	void on_activation(const std::string &msg);
	void on_login(const std::string &msg);
	void on_logout(const std::string &msg);
	void on_users_online(const std::vector<std::string> &users);
};

/***************************************************************************/

#endif // _yarmi__client_session_hpp
