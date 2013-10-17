
#ifndef _yarmi__chat__protocol_hpp
#define _yarmi__chat__protocol_hpp

#include <yarmi/yarmi.hpp>

YARMI_CONSTRUCT(
	client_invoker, // name of the client invoker
	((registration,
		((std::string, std::string)) // message : registration key
	))
	((activation,
		((std::string)) // message
	))
	((login,
		((std::string)) // message
	))
	((logout,
		((std::string)) // message
	))
	((users_online,
		((std::vector<std::string>)) // without args
	))
	,
	server_invoker, // name of the server invoker
	((registration,
		((std::string)) // username
	))
	((activation,
		((std::string, std::string, std::string)) // registration key : username : password
	))
	((login,
		((std::string, std::string)) // username : password
	))
	((logout,
		(()) // without args
	))
	((users_online,
		(()) // without args
	))
);

#endif // _yarmi__chat__protocol_hpp
