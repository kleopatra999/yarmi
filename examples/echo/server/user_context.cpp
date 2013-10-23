
#include "user_context.hpp"
#include "global_context.hpp"

/***************************************************************************/

user_context::user_context(boost::asio::io_service &ios, global_context<user_context> &gc)
	:yarmi::session_base(ios)
	,yarmi::server_invoker<user_context>(*this, *this)
	,gc(gc)
{}

/***************************************************************************/

void user_context::on_connected() {
	std::cout << "YARMI: on_connected(" << get_socket().remote_endpoint().address().to_string() << ") called" << std::endl;
}

void user_context::on_disconnected() {
	std::cout << "YARMI: on_disconnected() called" << std::endl;
}

void user_context::on_received(const char *ptr, std::size_t size) {
	invoke(ptr, size);
}

/***************************************************************************/

void user_context::on_ping(const std::string &msg) {
	std::cout << "received: \"" << msg << "\"" << std::endl;
	pong(msg);
	//throw std::runtime_error("remote exception");
}

/***************************************************************************/
