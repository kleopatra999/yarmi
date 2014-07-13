
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

#include "parser.hpp"
#include "protoinfo.hpp"
#include "tools.hpp"
#include "tokens.hpp"
#include "records.hpp"

#include <stdexcept>
#include <iostream>
#include <map>

namespace yarmigen {

/***************************************************************************/

void parse_one_proto(proto_info &pi, cursor &c) {
	// get proto type from 'proto(<type>)' section
	pi.type = get_proto_type(c);

	// check fo '{'
	check_next(c, proto_body_open_char);

	bool client = true;
	for ( ;; ) {
		cursor tc = c;
		const std::string kword = get_to_sep(c, ' ', proto_body_close_char);
		if ( !kword.empty() && kword[0] == ',' ) {
			nextch(c);
			client = false;
			continue;
		}
		if ( kword.empty() )
			break;

		record_ptr o = record_factory(kword);
		if ( !o )
			YARMIGEN_THROW(
				 "bad keyword \"%1%\" in %2%"
				,kword
				,tc.format()
			);

		o->parse(pi, c);
		//o->dump(std::cout);
		(client ? &pi.cl_records : &pi.sr_records)->push_back(std::move(o));
	}

	// check for '}'
	check_next(c, proto_body_close_char);
	// check for ';'
	check_next(c, proto_body_close_dotcomma_char);
}

/***************************************************************************/

std::vector<proto_info>
parse(const std::string &data) {
	cursor c(data.begin(), data.end());
	std::vector<proto_info> res;

	for ( ;; ) {
		proto_info pi;
		parse_one_proto(pi, c);

		res.push_back(std::move(pi));

		// end of file
		if ( std::distance(c.it, c.end) == 1 )
			break;
	}

	return std::move(res);
}

/***************************************************************************/

} // ns yarmigen
