
#include "client_session.hpp"

/***************************************************************************/

client_session::client_session(boost::asio::io_service &ios)
	:yarmi::client_base<client_session>(ios, this)
	,yarmi::client_invoker<client_session>(this, this)
{}

/***************************************************************************/

void client_session::on_registration(const std::string &msg, const std::string &regkey) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you activation key is: " << regkey << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_activation(const std::string &msg) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you successfully registered" << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_login(const std::string &msg) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you successfully logged in" << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_logout(const std::string &msg) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you successfully logged out" << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_users_online(const std::vector<std::string> &users) {
	if ( users.empty() ) {
		std::cout << "no users online" << std::endl;
		return;
	}

	std::cout << "users online:" << std::endl;
	for ( const auto &it: users ) {
		std::cout << "   " << it << std::endl;
	}
}

/***************************************************************************/
