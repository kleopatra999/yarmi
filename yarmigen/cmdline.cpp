
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
#include "throw.hpp"

#include <boost/program_options.hpp>

#include <map>

namespace yarmigen {

options parse_cmdline(int argc, char **argv) {
	using namespace boost::program_options;

	std::string protoname, resfname, reslang;

	options_description desc;
	desc.add_options()
		("help,h", "produce help message")
		("in,i", value<std::string>(&protoname)->required(), "proto file name")
		("out,o", value<std::string>(&resfname)->required(), "generated file name")
		("lang,l", value<std::string>(&reslang)->required(), "language of generated code(c, cpp, python, java, js)")
	;
	variables_map vars;
	store(command_line_parser(argc, argv).options(desc).run(), vars);
	notify(vars);

	static const std::map<std::string, e_lang> lang_map = {
		 {"c"			, e_lang::c			}
		,{"cpp"		, e_lang::cpp		}
		,{"python"	, e_lang::python	}
		,{"java"		, e_lang::java		}
		,{"js"		, e_lang::js		}
	};

	auto lang = lang_map.find(reslang);
	if ( lang == lang_map.end() ) {
		YARMIGEN_THROW("bad language(%s) of generated code", reslang);
	}

	return {protoname, resfname, lang->second};
}

} // ns yarmigen
