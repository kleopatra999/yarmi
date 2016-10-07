
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

#ifndef _yarmi__yarmi_fwd_hpp
#define _yarmi__yarmi_fwd_hpp

#include <yarmi/detail/cthash/fnv1a.hpp>
#include <yarmi/detail/endian/endian.hpp>

#include <memory>
#include <tuple>
#include <utility>

#include <boost/asio/ip/tcp.hpp>

namespace yarmi {

/***************************************************************************/

constexpr auto hash(const char *s) -> decltype(detail::fnv1a_32(s)) {
	return detail::fnv1a_32(s);
}
using call_id_type   = decltype(hash(""));
using call_name_type = const char*;

using buffer_ptr  = std::shared_ptr<char>;
using buffer_pair = std::pair<buffer_ptr, std::size_t>;

inline buffer_pair allocate_buffer(const std::size_t size) {
	buffer_ptr buffer(new char[size], [](char *p){delete[] p;});
	return {std::move(buffer), size};
}

struct server_statistic;
struct server_base;
struct session;
struct global_context_base;

using socket      = boost::asio::ip::tcp::socket;
using socket_ptr  = std::shared_ptr<socket>;
using session_ptr = std::shared_ptr<session>;

/***************************************************************************/

struct _serialize_only { _serialize_only() {} };
static const _serialize_only serialize_only;

/***************************************************************************/

namespace {

template<
	 typename CallID
	,typename CallName
	,typename... Args
	,std::size_t... Ids
>
inline std::tuple<const Args&...>
get_call_args_impl(const std::tuple<CallID, CallName, Args...> &args, std::index_sequence<Ids...>) {
	return std::tie(std::get<Ids+2>(args)...);
}

} // ns anon

template<typename Tuple>
inline auto get_call_id(const Tuple &tp) { return std::get<0>(tp); }

template<typename Tuple>
inline auto get_call_name(const Tuple &tp) { return std::get<1>(tp); }

struct call_id_name {
	call_id_type   call_id;
	call_name_type call_name;
};

template<typename Tuple>
inline call_id_name get_call_id_name(const Tuple &tp) {
	return {get_call_id(tp), get_call_name(tp)};
}

template<
	 typename CallID
	,typename CallName
	,typename... Args
>
inline std::tuple<const Args&...>
get_call_args(const std::tuple<CallID, CallName, Args...> &args) {
	return get_call_args_impl(args, std::index_sequence_for<Args...>());
}

/***************************************************************************/

struct invoker_base {
	virtual bool invoke(const call_id_type call_id, const buffer_pair &buffer) = 0;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__yarmi_fwd_hpp
