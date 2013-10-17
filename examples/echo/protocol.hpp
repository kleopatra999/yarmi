
#ifndef _yarmi__echo__protocol_hpp
#define _yarmi__echo__protocol_hpp

#include <yarmi/yarmi.hpp>

YARMI_CONSTRUCT(
	client_invoker, // name of the client invoker
	((pong,
		((std::string))
	)),
	server_invoker, // name of the server invoker
	((ping,
		((std::string))
	))
);

#endif // _yarmi__echo__protocol_hpp
