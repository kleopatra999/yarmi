
#include "command_processor.hpp"
#include "client_session.hpp"

#include <boost/thread/thread.hpp>

#include <iostream>

/***************************************************************************/

int main() {
	static const char *ip = "127.0.0.1";
	static const std::uint16_t port = 44550;

	boost::asio::io_service ios;
	client_session client(ios);
	client.connect(ip, port);

	command_processor processor(client);

	boost::thread thread([&ios]() {ios.run();});

	std::string cmd;
	while ( std::getline(std::cin, cmd) ) {
		if ( cmd == ":quit" ) break;
		processor.process(cmd);
	}

	client.disconnect();
	thread.join();

	return 0;
}

/***************************************************************************/
