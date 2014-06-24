
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

#include "cmdline.hpp"
#include "tools.hpp"
#include "throw.hpp"
#include "dump_info.hpp"
#include "reader.hpp"


#include "c_generator.hpp"
#include "cpp_generator.hpp"
#include "python_generator.hpp"
#include "java_generator.hpp"
#include "js_generator.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <cstring>

/***************************************************************************/

void generate(const std::string& cmdline, std::ostream &os, const std::vector<yarmigen::proto_info> &info, const yarmigen::e_lang lang) {
	using namespace yarmigen;

	static const std::map<e_lang, generator_t> map = {
		 {e_lang::c     , &c_generator     }
		,{e_lang::cpp   , &cpp_generator   }
		,{e_lang::python, &python_generator}
		,{e_lang::java  , &java_generator  }
		,{e_lang::js    , &js_generator    }
	};

	auto it = map.find(lang);
	if ( it == map.end() ) {
		YARMIGEN_THROW("bad language %s", options::str_lang_by_enum(lang));
	}

	it->second(cmdline, os, info);
}

/***************************************************************************/

std::string make_cmdline_string(char **argv) {
	#ifdef _WIN32
	#	define SEP '\\'
	#else
	#	define SEP '/'
	#endif

	const char *pn = std::strrchr(argv[0], SEP);
	pn = ( !pn ? "yarmigen" : pn+1);

	std::string res = pn;
	res.push_back(' ');

	for ( char **it = argv+1; *it; ++it ) {
		res += *it;
		res.push_back(' ');
	}

	return res;
}

/***************************************************************************/

int main(int argc, char **argv) {
	using namespace yarmigen;

	try {
		options opt = parse_cmdline(argc, argv);
		//opt.dump(std::cout);

		const std::string buf = read_file(opt.in);
		const std::vector<proto_info> info = read(buf);

		dump_info(std::cout, info);

		std::ofstream file(opt.out, std::ios::out|std::ios::trunc);
		if ( !file ) {
			std::cerr << "can't open output file" << std::endl;
			return 1;
		}

		const std::string cmdline = make_cmdline_string(argv);
		//std::cout << "cmdline='" << cmdline << "'" << std::endl;
		generate(cmdline, file, info, opt.lang);
	} catch (const std::exception &ex) {
		std::cerr << "[exception]: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

/***************************************************************************/
