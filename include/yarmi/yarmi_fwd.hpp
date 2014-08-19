
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

#ifndef _yarmi__yarmi_fwd_hpp
#define _yarmi__yarmi_fwd_hpp

#include <yarmi/detail/cthash/fnv1a.hpp>
#include <yarmi/detail/endian/endian.hpp>

#include <memory>

#include <boost/asio/ip/tcp.hpp>

namespace yarmi {

/***************************************************************************/

using call_id_type = decltype(yarmi::detail::fnv1a(""));
using buffer_ptr   = std::shared_ptr<char>;

using buffer_pair  = std::pair<buffer_ptr, std::size_t>;
inline buffer_pair allocate_buffer(const std::size_t size) {
	buffer_ptr buffer(new char[size], [](char *p){delete[] p;});
	return {std::move(buffer), size};
}

struct server_statistic;
struct server_base;
struct session;
struct global_context_base;

using socket_ptr  = std::shared_ptr<boost::asio::ip::tcp::socket>;
using session_ptr = std::shared_ptr<session>;

struct _serialize_only { _serialize_only() {} };
static const _serialize_only serialize_only;

struct _fake_log { _fake_log() {} };
static const _fake_log fake_log;

/***************************************************************************/

namespace detail {

template<typename Log, typename... Args>
auto logging(Log& log, const call_id_type call_id, const std::tuple<const Args&...> &args)
	-> decltype((log(call_id, args)), void())
{
	log(call_id, args);
}

template<typename Log, typename... Args>
void logging(Log&, const call_id_type, const std::tuple<const Args&...> &) {}

} // ns detail

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__yarmi_fwd_hpp
