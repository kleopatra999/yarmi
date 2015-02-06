Yet Another [RMI](http://en.wikipedia.org/wiki/Java_remote_method_invocation)
=========

RMI constructor using C++ preprocessor


Example
=========

```cpp
YARMI_CONSTRUCT(
	(yarmi),        // invokers namespace
	client_invoker, // name of the client invoker
	(enum(
		 e_status
		,std::uint8_t
		,
		(ok)
		(already_exists)
		(already_activated)
		(already_logged_in)
		(not_logged_in)
		(bad_username_or_password)
	)) /* enum */
	(struct(
        	mystruct
        	,
        	(a, int)
        	(b, double)
        	(c, std::pair<int, long>)
	)) /* struct */
	(proc(registration , on_registration, (std::string)                           )) // username
	(proc(activation   , on_activation  , (std::string, std::string, std::string) )) // registration key : username : password
	(proc(login        , on_login       , (std::string, std::string)              )) // username : password
	(proc(logout       , on_logout      , ()                                      )) // without args
	(proc(users_online , on_users_online, ()                                      )) // without args
	(proc(users_online , on_users_online, (std::string)                           )) // substring of username
	,
	server_invoker, // name of the server invoker
	(proc(registration , on_registration, (e_status, std::string)                 )) // status : registration key
	(proc(activation   , on_activation  , (e_status)                              )) // status
	(proc(login        , on_login       , (e_status)                              )) // status
	(proc(logout       , on_logout      , (e_status)                              )) // status
	(proc(users_online , on_users_online, (e_status, std::vector<std::string>)    )) // status : vector of usernames
);

```
