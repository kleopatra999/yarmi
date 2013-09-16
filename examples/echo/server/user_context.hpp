
#ifndef _yarmi__user_context_hpp
#define _yarmi__user_context_hpp

#include "protocol.hpp"

#include <yarmi/session_base.hpp>

/***************************************************************************/

template<typename>
struct global_context;

struct user_context: yarmi::session_base, yarmi::server_invoker<user_context> {
	user_context(boost::asio::io_service &ios, global_context<user_context> &gc);

	void on_connected();
	void on_disconnected();
	void on_received(const char *ptr, std::size_t size);

	void on_ping(const std::string &msg);

private:
	global_context<user_context> &gc;
};

/***************************************************************************/

#endif // _yarmi__user_context_hpp
