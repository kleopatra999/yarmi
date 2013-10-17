
#include "command_processor.hpp"
#include "client_session.hpp"

#include <unordered_map>
#include <functional>

/***************************************************************************/

struct command_processor::pimpl {
	pimpl(client_session &client)
		:client(client)
	{}

	client_session &client;

	std::unordered_map<std::string, std::function<void(const std::string&)>> cmds_map;
}; // struct pimpl

/***************************************************************************/

command_processor::command_processor(client_session &client)
	:impl(new pimpl(client))
{
	auto registration = [this](const std::string &cmd) {
		std::string username;
		while ( 1 ) {
			std::cout << "user name: ";
			std::cin >> username;
			if ( username.find(' ') == std::string::npos ) {
				break;
			} else {
				std::cout << "username can not contain spaces." << std::endl;
			}
		}

		impl->client.registration(username);
	};

	impl->cmds_map.emplace(":registration", registration);
}

command_processor::~command_processor()
{ delete impl; }

/***************************************************************************/

void command_processor::process(const std::string &text) {

}

/***************************************************************************/
