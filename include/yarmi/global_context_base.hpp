
#ifndef _yarmi__global_context_base_hpp
#define _yarmi__global_context_base_hpp

#include <yarmi/yarmi.hpp>

#include <boost/noncopyable.hpp>

#include <cstdint>
#include <memory>

namespace yarmi {

/***************************************************************************/

struct session_base;

struct global_context_base: private boost::noncopyable {
	global_context_base();
	virtual ~global_context_base();

	global_context_base& get_global_context_base();
	const global_context_base& get_global_context_base() const;

	/** add */
	std::uint64_t add_session(session_base *session);

	/** rm */
	void del_session(session_base *session);
	void del_session(std::int64_t id);

	/** manip */
	void set_id(session_base *session, std::int64_t id);

	/** test */
	bool has_session(session_base *session) const;
	bool has_session(std::int64_t id) const;

	/** get */
	session_base* get_session(std::int64_t id) const;

	/** info */
	std::size_t sessions() const;

	/** operations */
	void send_to(std::int64_t id, const yas::shared_buffer &buffer);

	template<typename Allocator, template<typename, typename> class container>
	void send_to(const container<std::int64_t, Allocator> &cont, const yas::shared_buffer &buffer) {
		for ( const auto &it: cont ) {
			send_to(it, buffer);
		}
	}

	void send_to_all(const session_base *session, const yas::shared_buffer &buffer);

private:
	struct pimpl;
	pimpl *impl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__global_context_base_hpp
