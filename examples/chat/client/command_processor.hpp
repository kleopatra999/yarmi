
#ifndef _yarmi__command_processor_hpp
#define _yarmi__command_processor_hpp

#include <boost/noncopyable.hpp>

#include <string>

/***************************************************************************/

struct client_session;

struct command_processor: private boost::noncopyable {
	command_processor(client_session &client);
	~command_processor();

	void process(const std::string &text);

private:
	struct pimpl;
	pimpl *impl;
};

/***************************************************************************/

#endif // _yarmi__command_processor_hpp
