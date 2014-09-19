
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

#ifndef _yarmi__detail__generate_struct__jsonify_hpp
#define _yarmi__detail__generate_struct__jsonify_hpp

#include <ostream>
#include <string>
#include <type_traits>

/***************************************************************************/

namespace yarmi{
namespace detail {

template<typename T>
struct _is_string: std::integral_constant<
	 bool
	,std::is_same<T, std::string>::value || std::is_same<T, const char*>::value
>
{};

template<typename T>
struct _is_bool: std::integral_constant<bool, std::is_same<T, bool>::value>
{};

template<typename T>
struct _is_char: std::integral_constant<
	 bool
	,(std::is_same<T, char>::value || std::is_same<T, unsigned char>::value || std::is_same<T, signed char>::value)
>
{};

template<typename T>
struct _is_not_string_bool_char: std::integral_constant<
	 bool
	,!(_is_string<T>::value || _is_bool<T>::value || _is_char<T>::value)
>
{};

/***************************************************************************/

// for string
template<typename T>
void jsonify(std::ostream& s, const T &o, typename std::enable_if<_is_string<T>::value>::type* = 0) {
	s << '\"' << o << '\"';
}
// for bool
template<typename T>
void jsonify(std::ostream& s, const T &o, typename std::enable_if<_is_bool<T>::value>::type* = 0) {
	 s << (o ? "true" : "false");
}
// for char
template<typename T>
void jsonify(std::ostream& s, const T &o, typename std::enable_if<_is_char<T>::value>::type* = 0) {
	 if ( std::is_signed<T>::value )
		  s << static_cast<char>(o);
	 else
		  s << static_cast<std::size_t>(o);
}
// for other types
template<typename T>
void jsonify(std::ostream& s, const T &o, typename std::enable_if<_is_not_string_bool_char<T>::value>::type* = 0) {
	s << o;
}

} // ns detail
} // ns yarmi

/***************************************************************************/

#endif // _yarmi__detail__generate_struct__jsonify_hpp
