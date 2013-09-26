
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

	/** operations */
	void send_to(std::int64_t id, const std::pair<std::shared_ptr<char>, std::size_t> &pair);
	template<typename T>
	void send_to(std::int64_t id, const T &o) { send_to(serialize(o)); }


	template<
		 typename Allocator
		,template<typename, typename> class container
	>
	void send_to(const container<std::int64_t, Allocator> &cont, const std::pair<std::shared_ptr<char>, std::size_t> &pair) {
		for ( const auto &it: cont ) {
			send_to(it, pair);
		}
	}
	template<
		 typename T
		,typename Allocator
		,template<typename, typename> class container
	>
	void send_to(const container<std::int64_t, Allocator> &cont, const T &o) {
		send_to(cont, serialize(o));
	}

	void send_to_all(const session_base *session, const std::pair<std::shared_ptr<char>, std::size_t> &pair);
	template<typename T>
	void send_to_all(const session_base *session, const T &o) {
		send_to_all(session, serialize(o));
	}

private:
	template<typename T>
	std::pair<std::shared_ptr<char>, std::size_t>
	serialize(const T &o) {
		YARMI_OARCHIVE_TYPE oa(yas::no_header);
		oa & o;
		YARMI_OARCHIVE_TYPE pa;
		pa & oa.get_intrusive_buffer();
		const yas::shared_buffer &buffer = pa.get_shared_buffer();
		return std::make_pair(buffer.data, buffer.size);
	}

private:
	struct pimpl;
	pimpl *impl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__global_context_base_hpp
