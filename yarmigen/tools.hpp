
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

#include <yarmi/detail/throw/throw.hpp>
#include "cursor.hpp"

#include <string>
#include <iosfwd>

namespace yarmigen {

/***************************************************************************/

enum class e_lang;

std::string ext_by_type(const e_lang lang);

std::string read_file(std::ifstream &);

/***************************************************************************/

char nextch(cursor &c);
char prevch(cursor &c);
char curch(cursor &c);
void skipws(cursor &c);
void check_next(cursor &c, const char ch);

template<std::size_t N>
void check_substring(cursor &c, const char(&str)[N]) {
	skipws(c);
	static_assert(N > 2, "N > 2");
	for ( std::size_t idx = 0; idx < N-1; ++idx ) {
		const char ch = nextch(c);
		if ( ch != str[idx] )
			YARMI_THROW(
				 "unexpected character. expected '%c' get '%c', %s"
				,ch
				,str[idx]
				,c.format()
			);
	}
}

template<typename... Seps>
std::string get_to_sep(cursor &c, char sep, Seps... seps) {
	skipws(c);
	std::string res;

	for ( char ch = nextch(c); ; ch = nextch(c) ) {
		if ( ch == '/' ) {
			prevch(c);
			skipws(c);
			continue;
		}
		bool flag = false;
		auto func = [](bool &f, char ch, char sep) { return f=f || ch == sep; };
		auto apply= [](...) {};

		apply(func(flag, ch, sep), func(flag, ch, seps)...);
		if ( flag ) break;

		res.push_back(ch);
	}

	prevch(c);

	return res;
}

/***************************************************************************/

template<typename... Chars>
std::string erase_chars(const std::string &str, char ch, Chars... chars) {
	std::string res;

	for ( auto c: str ) {
		bool flag = false;
		auto func = [](bool &f, char c, char ch) { return f=f || c == ch; };
		auto apply= [](...) {};

		apply(func(flag, c, ch), func(flag, c, chars)...);
		if ( flag ) continue;

		res.push_back(c);
	}

	return res;
}

std::vector<std::string>
split_template_args(const std::string &str);

/***************************************************************************/

struct proto_info;
bool struct_already_declared(const proto_info &pi, const std::string &name);

/***************************************************************************/

} // ns yarmigen
