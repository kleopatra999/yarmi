
// Copyright (c) 2013-2016, niXman (i dotty nixman doggy gmail dotty com)
// All rights reserved.
//
// This file is part of YARMI(https://github.com/niXman/yarmi) project.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or
//   other materials provided with the distribution.
//
//   Neither the name of the {organization} nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef _yarmi__server__global_context_base_hpp
#define _yarmi__server__global_context_base_hpp

#include <yarmi/yarmi.hpp>
#include <yarmi/server/server_statistic.hpp>

#include <boost/noncopyable.hpp>

#include <cstdint>
#include <memory>

namespace yarmi {

/***************************************************************************/

struct session;

struct global_context_base: private boost::noncopyable {
	global_context_base();
	virtual ~global_context_base();

	global_context_base& get_global_context_base();
	const global_context_base& get_global_context_base() const;

	/** add */
	std::uint64_t add_session(session *session);

	/** get */
	session* get_session(std::int64_t id) const;

	/** rm */
	void del_session(session *session);
	void del_session(std::int64_t id);

	/** manip */
	void set_id(session *session, std::int64_t id);

	/** test */
	bool has_session(session *session) const;
	bool has_session(std::int64_t id) const;

	/** info */
	std::size_t sessions() const;

	/** operations */
	void send_to(std::int64_t id, const buffer_pair &buffer);

	template<typename Iter>
	void send_to(Iter beg, Iter end, const buffer_pair &buffer) {
		for ( ; beg != end; ++beg ) {
			send_to(*beg, buffer);
		}
	}

	void send_to_all(const session *exclude, const buffer_pair &buffer);

private:
	struct impl;
	impl *pimpl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__server__global_context_base_hpp
