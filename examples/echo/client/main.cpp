
#include <protocol.hpp>

#include <yarmi/client.hpp>

#include <iostream>

/***************************************************************************/

struct client: yarmi::client_base<client>, yarmi::client_invoker<client> {
	client(boost::asio::io_service &ios)
		:yarmi::client_base<client>(ios, this)
		,yarmi::client_invoker<client>(this, this)
		,msg_index(0)
	{}

	void on_pong(const std::string &msg) {
		std::cout << "received: \"" << msg << "\"" << std::endl;
		ping("my message "+std::to_string(++msg_index));
	}

	std::size_t msg_index;
};

/***************************************************************************/

int main() {
	static const char *ip = "127.0.0.1";
	static const std::uint16_t port = 44550;

	boost::asio::io_service ios;
	client c(ios);
	c.connect(ip, port);
	c.ping("message");

	ios.run();

	return 0;
}

/***************************************************************************/
