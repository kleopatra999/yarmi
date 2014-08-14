
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

#ifndef _yarmi__get_proc_name_hpp
#define _yarmi__get_proc_name_hpp

#include <yarmi/yarmi_fwd.hpp>

namespace yarmi {
namespace detail {

enum class request_or_handler { request, handler };

template<typename Invoker, typename... Invokers>
const char* get_proc_name(request_or_handler d, call_id_type call_id, const Invoker &head, const Invokers&... tail) {
	const char *res = 0;
	bool flag  = false;
	auto apply = [](...) {};
	auto func  = [&flag, &res, d](call_id_type call_id, const Invoker &invoker) {
		return flag=flag || (
			d==request_or_handler::request
				? (res=invoker.meta_request_name(call_id))
				: (res=invoker.meta_handler_name(call_id))
		);
	};
	apply(func(call_id, head), func(call_id, tail)...);

	return res;
}

} // ns detail

/***************************************************************************/

template<typename Invoker, typename... Invokers>
const char* get_request_name(call_id_type call_id, const Invoker &invoker, const Invokers&... invokers) {
	return detail::get_proc_name(detail::request_or_handler::request, call_id, invoker, invokers...);
}

template<typename Invoker, typename... Invokers>
const char* get_handler_name(call_id_type call_id, const Invoker &invoker, const Invokers&... invokers) {
	return detail::get_proc_name(detail::request_or_handler::handler, call_id, invoker, invokers...);
}

template<typename Invoker, typename... Invokers>
const char* get_proc_name(call_id_type call_id, const Invoker &invoker, const Invokers&... invokers) {
	const char *r = get_request_name(call_id, invoker, invokers...);
	if ( r ) return r;

	const char *h = get_handler_name(call_id, invoker, invokers...);
	return h;
}

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__get_proc_name_hpp
