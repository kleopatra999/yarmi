
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

#include "cmdline.hpp"
#include "tools.hpp"
#include "dump_info.hpp"
#include "parser.hpp"

#include "c_generator.hpp"
#include "cpp_generator.hpp"
#include "python_generator.hpp"
#include "java_generator.hpp"
#include "js_generator.hpp"

#include "checks.hpp"

#include <yarmi/detail/throw/throw.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

/***************************************************************************/

void generate(std::ostream &os, const std::vector<yarmigen::proto_info> &info, const yarmigen::e_lang lang) {
	using namespace yarmigen;
	static const generator_t gens[] = {
		 &c_generator
		,&cpp_generator
		,&python_generator
		,&java_generator
		,&js_generator
	};

	const std::size_t idx = static_cast<std::size_t>(lang);
	if ( idx > sizeof(gens)/sizeof(gens[0]) )
		YARMI_THROW("bad language index(%1%) for language %2%", idx, options::str_lang_by_enum(lang));

	gens[idx](os, info);
}

/***************************************************************************/

int main(int argc, char **argv) {
	using namespace yarmigen;

	try {
		const options opt = parse_cmdline(argc, argv);
		//opt.dump(std::cout);

		std::ifstream ifile(opt.in);
		if ( !ifile )
			YARMI_THROW("can't open input file \""+opt.in+"\"");

		const std::string buf = read_file(ifile);
		const std::vector<proto_info> info = parse(buf);

		std::cout << "checking...";
		full_check(info);
		std::cout << "done!" << std::endl;
		//dump_info(std::cout, info);

		std::ofstream ofile(opt.out, std::ios::out|std::ios::trunc);
		if ( !ofile ) {
			std::cerr << "can't create output file \"" << opt.out << "\"" << std::endl;
			return 1;
		}

		generate(ofile, info, opt.lang);
	} catch (const std::exception &ex) {
		std::cerr << "[exception]: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

/***************************************************************************/
