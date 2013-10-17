
#include <protocol.hpp>

#include "global_context.hpp"
#include "user_context.hpp"

#include <yarmi/server.hpp>

#include <iostream>

/***************************************************************************/

int main() {
	boost::asio::io_service ios;

	global_context<user_context> gc(
		 "./users.db"
		,"./messages.db"
	);

	yarmi::server<user_context, global_context> server(
		 "127.0.0.1"
		 ,44550
		 ,ios
		 ,gc
	);
	server.start();

	ios.run();
}

/***************************************************************************/
