
// Copyright (c) 2013,2014, niXman (i dotty nixman doggy gmail dotty com)
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

#include <yarmi/server/global_context_base.hpp>
#include <yarmi/server/session.hpp>
#include <yarmi/detail/throw/throw.hpp>

#include <stdexcept>
#include <sstream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

namespace yarmi {

/***************************************************************************/

struct session_wrapper {
	session *sptr;
	std::int64_t id;

	struct by_session;
	struct by_id;
};

struct global_context_base::impl {
	impl()
		:session_ids(-1)
		,sessions()
	{}

	typedef boost::multi_index::multi_index_container<
		 session_wrapper
		,boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				 boost::multi_index::tag<session_wrapper::by_session>
				,boost::multi_index::member<session_wrapper, session*, &session_wrapper::sptr>
			>
			,boost::multi_index::ordered_unique<
				 boost::multi_index::tag<session_wrapper::by_id>
				,boost::multi_index::member<session_wrapper, std::int64_t, &session_wrapper::id>
			>
		>
	> sessions_cont;

	std::int64_t session_ids;
	sessions_cont sessions;
}; // struct pimpl

/***************************************************************************/

global_context_base::global_context_base()
	:pimpl(new impl)
{}

global_context_base::~global_context_base() {
	delete pimpl;
}

global_context_base &global_context_base::get_global_context_base() { return *this; }
const global_context_base &global_context_base::get_global_context_base() const { return *this; }

/***************************************************************************/

std::uint64_t global_context_base::add_session(session *session) {
	if ( has_session(session) ) {
		YARMI_THROW("session \"%1%\" already in sessions list", session);
	}

	std::int64_t idx = pimpl->session_ids--;
	pimpl->sessions.insert(session_wrapper{session, idx});

	return idx;
}

void global_context_base::del_session(session *session) {
	if ( !has_session(session) ) {
		YARMI_THROW("session \"%1%\" not in sessions list", session);
	}

	auto &index = pimpl->sessions.get<session_wrapper::by_session>();
	auto it = index.find(session);
	index.erase(it);
}

void global_context_base::del_session(std::int64_t id) {
	if ( !has_session(id) ) {
		YARMI_THROW("session with ID \"%1%\" not in sessions list", id);
	}

	auto &index = pimpl->sessions.get<session_wrapper::by_id>();
	auto it = index.find(id);
	index.erase(it);
}

/***************************************************************************/

void global_context_base::set_id(session *session, std::int64_t id) {
	auto &index = pimpl->sessions.get<session_wrapper::by_session>();

	auto it = index.find(session);
	if ( it == index.end() ) {
		YARMI_THROW("YARMI: session \"%1%\" not in sessions list", session);
	}

	index.erase(it);

	pimpl->sessions.insert(session_wrapper{session, id});
}

/***************************************************************************/

bool global_context_base::has_session(session *session) const {
	const auto &index = pimpl->sessions.get<session_wrapper::by_session>();
	return index.find(session) != index.end();
}

bool global_context_base::has_session(std::int64_t id) const {
	const auto &index = pimpl->sessions.get<session_wrapper::by_id>();
	return index.find(id) != index.end();
}

session* global_context_base::get_session(std::int64_t id) const {
	if ( ! has_session(id) ) return 0;

	const auto it = pimpl->sessions.get<session_wrapper::by_id>().find(id);
	return it->sptr;
}

/***************************************************************************/

std::size_t global_context_base::sessions() const {
	return pimpl->sessions.size();
}

/***************************************************************************/

void global_context_base::send_to(std::int64_t id, const buffer_pair &buffer) {
	const auto it = pimpl->sessions.get<session_wrapper::by_id>().find(id);
	if ( it == pimpl->sessions.get<session_wrapper::by_id>().end() ) return;
	it->sptr->send(buffer);
}

void global_context_base::send_to_all(const session *exclude, const buffer_pair &buffer) {
	for ( const auto &it: pimpl->sessions ) {
		if ( it.sptr == exclude ) continue;
		it.sptr->send(buffer);
	}
}

/***************************************************************************/

} // ns yarmi
