
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

#ifndef _yarmi__detail__generate_struct__tuple_hpp
#define _yarmi__detail__generate_struct__tuple_hpp

#include <cxxabi.h>
#include <typeinfo>
#include <cstring>
#include <cstdlib>

#include <ostream>
#include <tuple>

#include <yarmi/detail/generate_struct/decorators.hpp>
#include <yarmi/detail/generate_struct/jsonify.hpp>

/***************************************************************************/

namespace  {

template<typename T>
const char* demangle()  {
	int stat;
	static char str[1024] = {0};
	char* ptr = abi::__cxa_demangle(typeid(T).name(), 0, 0, &stat);
	if ( !ptr ) return "NULL";
	std::strncpy(str, ptr, sizeof(str));
	::free(ptr);
	return str;
}

/***************************************************************************/

namespace types {
struct zero {};
struct one {};
struct some {};
} // ns types

/***************************************************************************/

template<std::size_t idx, typename... Args>
void print_item(std::ostream& s, const std::tuple<Args...> &o, std::true_type) {
	s << '\"' << demangle<typename std::tuple_element<idx, std::tuple<Args...>>::type>() << "\":";
	yarmi::detail::jsonify(s, std::get<idx>(o));
}

template<std::size_t idx, typename... Args>
void print_item(std::ostream& s, const std::tuple<Args...> &o, std::false_type) {
	s << '\"' << demangle<typename std::tuple_element<idx, std::tuple<Args...>>::type>() << "\":";
	yarmi::detail::jsonify(s, std::get<idx>(o));
	s << yarmi::detail::default_delimiter;
	print_item<idx + 1>(s, o, std::integral_constant<bool, idx + 1 == sizeof...(Args) - 1>());
}

template<typename ... Args>
void print_tuple(std::ostream&, const std::tuple<Args...>&, types::zero) {}

template<typename ... Args>
void print_tuple(std::ostream& s, const std::tuple<Args...>& o, types::one) {
	yarmi::detail::jsonify(s, std::get<0>(o));
}

template<typename ... Args>
void print_tuple(std::ostream& s, const std::tuple<Args...>& o, types::some) {
	print_item<0>(s, o, std::false_type());
}

} // ns

/***************************************************************************/

namespace yarmi{
namespace detail {

template<typename... Args>
std::ostream& operator<<(std::ostream& s, const std::tuple<Args...>& o) {
	using tuple_size = typename std::conditional<
		 sizeof...(Args) == 0
		,types::zero
		,typename std::conditional<
			 sizeof...(Args) == 1
			,types::one
			,types::some
		>::type
	>::type;

	s << detail::object_open_symbol;
	print_tuple(s, o, tuple_size());

	return s << detail::object_close_symbol;
}

} // ns detail
} // ns yarmi

/***************************************************************************/

#endif // _yarmi__detail__generate_struct__tuple_hpp
