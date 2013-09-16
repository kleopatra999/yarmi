
#ifndef _yarmi__global_context_hpp
#define _yarmi__global_context_hpp

#include <yarmi/global_context_base.hpp>

#include <string>

struct user_context;

/***************************************************************************/

template<typename UC>
struct global_context: yarmi::global_context_base {
	global_context(const std::string &users_db, const std::string &messages_db);
	~global_context();

	bool user_registered(const std::string &username) const;
	bool user_activated(const std::string &username) const;
	bool user_logged_in(const std::string &username) const;

	void registration(const std::string &username);
	void activation(const std::string &regkey, const std::string &username, const std::string &password);
	void login(const std::string &username, const std::string &password);
	void logout();
	void users_online();

private:
	struct pimpl;
	pimpl *impl;
};

/***************************************************************************/

#endif // _yarmi__global_context_hpp
