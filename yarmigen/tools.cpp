
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

#include <yarmi/throw.hpp>
#include "options.hpp"
#include "cursor.hpp"
#include "protoinfo.hpp"

#include <fstream>

namespace yarmigen {

/***************************************************************************/

char nextch(cursor &c) {
	if ( c.it == c.end )
		YARMI_THROW("end of file exceeded in %s", c.format());

	c.inc_column();
	return *(c.it++);
}

char prevch(cursor &c) {
	--(c.it);
	c.dec_column();
	return *(c.it);
}

char curch(cursor &c) {
	return *(c.it);
}

void skipws(cursor &c) {
	for ( ;; ) {
		char ch = nextch(c);
		switch ( ch ) {
			case '/':
				if ( (ch=curch(c)) != '/' )
					YARMI_THROW(
						"bad char '%c' in %s"
						,ch
						,c.format()
					);
				while ( (ch=nextch(c)) != '\n' );

			case '\n':
				c.inc_line();
				break;

			case ' ':
			case '\t':
			case '\r':
				 break;

			default:
				--(c.it);
				c.dec_column();
				return;
		}
	}
}

void check_next(cursor &c, const char ch) {
	skipws(c);
	const char ch2 = nextch(c);
	if ( ch != ch2 )
		YARMI_THROW(
			 "next char error in %s, expected '%c' get '%c'"
			,c.format()
			,ch
			,ch2
		);
}

std::vector<std::string>
split_template_args(const std::string &str) {
	std::string arg;
	std::vector<std::string> res;

	int open = 0;
	auto it = str.begin(), end = str.end();
	cursor c(str.begin(), str.end());

	for ( ; it != end; ) {
		const char ch = nextch(c);
		if ( ch == '<' || ch == '>' ) {
			open += (ch == '<' ? 1 : -1);
		}
		if ( ch == ',' && open == 0 ) {
			res.push_back(arg);
			arg.clear();
			skipws(c);
			continue;
		}
		arg.push_back(ch);
	}

	if ( it == end )
		res.push_back(arg);

	return res;
}

/***************************************************************************/

std::string ext_by_type(const yarmigen::e_lang lang) {
	static const char *ext[] = {
		 "c"      // options::e_lang::c      -> 0
		,"cpp"    // options::e_lang::cpp    -> 1
		,"python" // options::e_lang::python -> 2
		,"java"   // options::e_lang::java   -> 3
		,"js"     // options::e_lang::js     -> 4
	};

	return ext[static_cast<int>(lang)];
}

/***************************************************************************/

std::string read_file(std::ifstream &file) {
	file.unsetf(std::ios::skipws);

	const std::string buf(
		 (std::istreambuf_iterator<char>(file))
		,(std::istreambuf_iterator<char>())
	);

	return std::move(buf);
}

/***************************************************************************/

bool struct_already_declared(const proto_info &pi, const std::string &name) {
	auto beg = pi.cl_records.begin();
	auto end = pi.cl_records.end();
	for ( ; beg != end; ++beg ) {
		if ( (*beg)->type() == record_type::struct_ && (*beg)->name() == name )
			return true;
	}
	beg = pi.sr_records.begin();
	end = pi.sr_records.end();
	for ( ; beg != end; ++beg ) {
		if ( (*beg)->type() == record_type::struct_ && (*beg)->name() == name )
			return true;
	}

	return false;
}

/***************************************************************************/

} // ns yarmigen
