
#ifndef _yarmi__global_context_base_hpp
#define _yarmi__global_context_base_hpp

#include <boost/noncopyable.hpp>

#include <cstdint>
#include <memory>

namespace yarmi {

/***************************************************************************/

struct session_base;

struct global_context_base: private boost::noncopyable {
	global_context_base();
	~global_context_base();

	global_context_base& get_global_context_base();
	const global_context_base& get_global_context_base() const;

	/** add/rm */
	std::uint64_t add_session(session_base *session);
	void del_session(session_base *session);
	void del_session(std::uint64_t id);
	void set_id(session_base *session, std::uint64_t id);

	/** test */
	bool has_session(session_base *session) const;
	bool has_session(std::uint64_t id) const;

	/** info */
	std::size_t sessions() const;

	/** operation */
	void send_to_all(const std::shared_ptr<char> &buffer, std::size_t size);

	/** IO stats */
	void inc_recv_bytes(std::uint64_t bytes);
	void inc_sent_bytes(std::uint64_t bytes);
	std::uint64_t recv_bytes() const;
	std::uint64_t sent_bytes() const;
	void inc_requests();
	void inc_replies();
	std::uint64_t requests() const;
	std::uint64_t replies() const;

private:
	struct pimpl;
	pimpl *impl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__global_context_base_hpp
