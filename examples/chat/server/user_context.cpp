
#include "global_context.hpp"
#include "user_context.hpp"

/***************************************************************************/

user_context::user_context(boost::asio::io_service &ios, global_context<user_context> &gc)
	:yarmi::session_base(ios)
	,yarmi::server_invoker<user_context>(*this, *this)
	,gc(gc)
{}

/***************************************************************************/

void user_context::on_connected() {

}

/***************************************************************************/

void user_context::on_disconnected() {

}

/***************************************************************************/

void user_context::on_received(const char *ptr, std::size_t size) {
	invoke(ptr, size);
}

/***************************************************************************/

void user_context::on_registration(const std::string &username) {
	if ( gc.user_registered(username) ) {
		registration("username "+username+" already registered", "");
	} else {

	}
}

/***************************************************************************/

void user_context::on_activation(const std::string &regkey, const std::string &useranme, const std::string &pass) {

}

/***************************************************************************/

void user_context::on_login(const std::string &useranme, const std::string &pass) {

}

/***************************************************************************/

void user_context::on_logout() {

}

/***************************************************************************/

void user_context::on_users_online() {

}

/***************************************************************************/
