
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

#include "throw.hpp"
#include "reader.hpp"
#include "protoinfo.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>

namespace yarmigen {

/***************************************************************************/

#define YARMIGEN_DECL_C(name, val) \
	static constexpr const char name = val

#define YARMIGEN_DECL_S(name, val) \
	static constexpr const char name[] = val

/***************************************************************************/

YARMIGEN_DECL_S(proto_str, "proto");

YARMIGEN_DECL_C(proto_str_open_char, '(');
YARMIGEN_DECL_C(proto_str_close_char, ')');

YARMIGEN_DECL_S(proto_type_api_str, "api");
YARMIGEN_DECL_S(proto_type_service_str, "service");

YARMIGEN_DECL_C(proto_ns_cl_open_char, '[');
YARMIGEN_DECL_C(proto_ns_cl_close_char, ']');
YARMIGEN_DECL_C(proto_ns_cl_tag_separator, ':');
YARMIGEN_DECL_S(proto_ns_cl_namespace_str, "namespace");
YARMIGEN_DECL_S(proto_ns_cl_class_str, "class");

YARMIGEN_DECL_C(proto_open_body_char, '{');
YARMIGEN_DECL_C(proto_close_body_char, '}');

YARMIGEN_DECL_C(proto_proc_open_char, '[');
YARMIGEN_DECL_C(proto_proc_close_char, ']');
YARMIGEN_DECL_C(proto_proc_sig_open_char, '(');
YARMIGEN_DECL_C(proto_proc_sig_close_char, ')');

/***************************************************************************/

using iterator = std::string::const_iterator;

/***************************************************************************/

struct cursor {
	cursor()
		:line(1)
		,col(0)
	{}

	void next_line() {
		++line;
		col = 0;
	}
	void next_column() { ++col; }

	std::string format() const {
		return "line " + std::to_string(line) + " column " + std::to_string(col);
	}

	std::size_t line;
	std::size_t col;
};

/***************************************************************************/

char nextch(iterator &it, iterator end, cursor &d) {
	if ( it == end )
		YARMIGEN_THROW("end of file exceeded in %s", d.format());

	d.next_column();

	return *it++;
}

void check_next(iterator &it, iterator end, const char ch, cursor &d) {
	const char ch2 = nextch(it, end, d);
	if ( ch != ch2 ) {
		YARMIGEN_THROW("next char error in %s, expected '%c' get '%c'", d.format(), ch, ch2);
	}
}

void skip_whitespace(iterator &it, iterator end, cursor &d) {
	for ( ;; ) {
		char ch = nextch(it, end, d);
		switch ( ch ) {
			case '/':
				check_next(it, end, '/', d);
				while ( (ch=nextch(it, end, d)) != '\n' );

			case '\n':
				d.next_line();
				break;

			case ' ':
			case '\t':
			case '\r':
				 break;

			default:
				--it;
				--d.col;
				return;
		}
	}
}

/***************************************************************************/

template<std::size_t N>
void check_substring(const char(&str)[N], iterator &it, iterator end, cursor &d) {
	static_assert(N > 2, "N > 2");
	for ( std::size_t idx = 0; idx < N-1; ++idx ) {
		const char ch = nextch(it, end, d);
		if ( ch != str[idx] )
			YARMIGEN_THROW("unexpected character. expected '%c' get '%c', %s", ch, str[idx], d.format());
	}
}

std::string get_proto_type(iterator &it, iterator end, cursor &d) {
	check_substring(proto_str, it, end, d);
	check_next(it, end, proto_str_open_char, d);
	skip_whitespace(it, end, d);

	std::string res;
	for (char ch = nextch(it, end, d);
		  ch != proto_str_close_char;
		  ch = nextch(it, end, d)
	) { res.push_back(ch); }

	return res;
}

/***************************************************************************/

template<typename... Objs>
void apply(Objs...) {}

template<typename... Seps>
std::string get_to_sep(iterator &it, iterator end, cursor &d, char sep, Seps... seps) {
	std::string res;
	char ch = nextch(it, end, d);
	for ( ;; ch = nextch(it, end, d) ) {
		bool flag = false;
		auto func = [&flag](char ch, char sep) { return flag=flag || ch == sep; };

		apply(func(ch, sep), func(ch, seps)...);
		if ( flag ) break;

		res.push_back(ch);
	}
	if ( ch == ',' )
		--it;

	return res;
}

/***************************************************************************/

std::pair<std::string, std::string>
get_ns_and_class_names(iterator &it, iterator end, cursor &c) {
	skip_whitespace(it, end, c);
	const std::string tag1 = get_to_sep(
		 it
		,end
		,c
		,' '
		,proto_ns_cl_tag_separator
		,proto_ns_cl_close_char
		,'\n'
	);
	if ( tag1 != proto_ns_cl_namespace_str && tag1 != proto_ns_cl_class_str ) {
		YARMIGEN_THROW("'%s' or '%s' tags should be in %d"
			,proto_ns_cl_namespace_str
			,proto_ns_cl_class_str
			,c.line
		);
	}

//	std::cout << "s1=|" << s1 << '|' << std::endl << std::flush;
	skip_whitespace(it, end, c);
	const std::string val1 = get_to_sep(
		 it
		,end
		,c
		,' '
		,proto_ns_cl_tag_separator
		,proto_ns_cl_close_char
		,'\n'
	);
//	std::cout << "s2=|" << s2 << '|' << std::endl << std::flush;

	skip_whitespace(it, end, c);
	check_next(it, end, ',', c);
	skip_whitespace(it, end, c);

	skip_whitespace(it, end, c);
	const std::string tag2 = get_to_sep(
		 it
		,end
		,c
		,' '
		,proto_ns_cl_tag_separator
		,proto_ns_cl_close_char
		,'\n'
	);
	if ( tag2 != proto_ns_cl_namespace_str && tag2 != proto_ns_cl_class_str ) {
		YARMIGEN_THROW("'%s' or '%s' tags should be in %d"
			,proto_ns_cl_namespace_str
			,proto_ns_cl_class_str
			,c.line
		);
	}

	//	std::cout << "s3=|" << s3 << '|' << std::endl << std::flush;
	skip_whitespace(it, end, c);
	const std::string val2 = get_to_sep(
		 it
		,end
		,c
		,' '
		,proto_ns_cl_tag_separator
		,proto_ns_cl_close_char
		,'\n'
	);
//	std::cout << "s4=|" << s4 << '|' << std::endl << std::flush;

	skip_whitespace(it, end, c);

	return {val1, val2};
}

/***************************************************************************/

std::vector<std::string>
split_args(const std::string &str) {
	cursor d;
	std::string arg;
	std::vector<std::string> res;

	int open = 0;
	auto it = str.begin(), end = str.end();

	for ( ; it != end; ) {
		const char ch = nextch(it, end, d);
		if ( ch == '<' || ch == '>' ) {
			open += (ch == '<' ? 1 : -1);
		}
		if ( ch == ',' && open == 0 ) {
			res.push_back(arg);
			arg.clear();
			skip_whitespace(it, end, d);
			continue;
		}
		arg.push_back(ch);
	}

	if ( it == end )
		res.push_back(arg);

	return res;
}

proc_info get_proc_info(iterator &it, iterator end, cursor &d) {
	proc_info res;

//	std::cout << "*it=" << *it << std::endl << std::flush;

	// check for '[' (start proc section)
	check_next(it, end, proto_proc_open_char, d);
	skip_whitespace(it, end, d);

	//std::cout << "*it=" << *it << std::endl << std::flush;

	res.request = get_to_sep(it, end, d, ' ', ',');
//	std::cout << "request: " << res.request << std::endl;
	skip_whitespace(it, end, d);

//	std::cout << "*it=" << *it << std::endl << std::flush;
	check_next(it, end, ',', d);
	skip_whitespace(it, end, d);

//	std::cout << "*it=" << *it << std::endl << std::flush;
	res.handler = get_to_sep(it, end, d, ' ', ',');
//	std::cout << "handler: " << res.handler << std::endl;
//	std::cout << "*it=" << *it << std::endl << std::flush;
	skip_whitespace(it, end, d);

//	std::cout << "*it=" << *it << std::endl << std::flush;

	// request | handler comma separator
	check_next(it, end, ',', d);
	skip_whitespace(it, end, d);

	// check for '(' (start args section)
	check_next(it, end, proto_proc_sig_open_char, d);
	skip_whitespace(it, end, d);

//	std::cout << "*it=" << *it << std::endl << std::flush;
	const std::string args = get_to_sep(it, end, d, proto_proc_sig_close_char);
	res.args = split_args(args);
//	res.dump(std::cout);
//	std::cout << std::endl << std::flush;

	skip_whitespace(it, end, d);

	// check for ']' (end proc section)
	check_next(it, end, proto_proc_close_char, d);
	skip_whitespace(it, end, d);

	return res;
}

/***************************************************************************/
void proto_section_switch(
	 proto_info &info
	,int client_or_server
	,const std::string &namespace_
	,const std::string &class_
	,const std::vector<proc_info> &procs
) {
	if ( client_or_server == 0 ) {
		info.req_namespace_ = namespace_;
		info.req_class_ = class_;
		info.req_procs_ = procs;
	} else {
		info.rep_namespace_ = namespace_;
		info.rep_class_ = class_;
		info.rep_procs_ = procs;
	}
}

void parse_one_section(proto_info &info, int cs, iterator &it, iterator end, cursor &d) {
	skip_whitespace(it, end, d);
	// check for '[' (namspace and class_name section)
	check_next(it, end, proto_ns_cl_open_char, d);

	// extract namespace and class_name
	const std::pair<std::string, std::string> ns_cn = get_ns_and_class_names(it, end, d);
//	std::cout << "namespace=" << ns_cn.first << ", class=" << ns_cn.second << std::endl << std::flush;

	// check for ']'
	check_next(it, end, proto_ns_cl_close_char, d);
	skip_whitespace(it, end, d);

	// process proc's sections
	std::vector<proc_info> procs;
	for ( ;; ) {
		//std::cout << "*it=" << *it << std::endl << std::flush;
		proc_info proc = get_proc_info(it, end, d);
		procs.push_back(proc);
		skip_whitespace(it, end, d);

		if ( cs == 0 ) {
			if ( *it == ',' )
				break;
		} else {
			if ( *it == proto_close_body_char )
				break;
		}
	}

	proto_section_switch(info, cs, ns_cn.first, ns_cn.second, procs);
}

proto_info parse_one_proto(iterator &it, iterator end, cursor &d) {
	proto_info res;

	// get proto type from 'proto(<type>)' section
//	std::cout << "*it=" << *it << std::endl << std::flush;
	const std::string proto_type = get_proto_type(it, end, d);
//	std::cout << "proto type=" << proto_type << std::endl;
	res.type_ = (proto_type == proto_type_api_str ? info_type::api : info_type::service);

	skip_whitespace(it, end, d);
	// check fo '{'
	check_next(it, end, proto_open_body_char, d);

	if ( res.type_ == info_type::api ) {
		parse_one_section(res, 0 /* client */, it, end, d);
		check_next(it, end, ',', d);
		parse_one_section(res, 1 /* server */, it, end, d);
	} else {
		parse_one_section(res, 0 /* client */, it, end, d);
	}

	skip_whitespace(it, end, d);
	check_next(it, end, proto_close_body_char, d);

	return res;
}

/***************************************************************************/

std::vector<proto_info> read(const std::string &str) {
	std::vector<proto_info> res;

	cursor d;
	auto it = str.begin(), end = str.end();

	for ( ;; ) {
		skip_whitespace(it, end, d);
		proto_info info = parse_one_proto(it, end, d);

		res.push_back(info);

		//std::cout << "*it=" << *it << std::endl << std::flush;
		if ( it+1 == end )
			break;
	}

	return res;
}

/***************************************************************************/

} // ns yarmigen
