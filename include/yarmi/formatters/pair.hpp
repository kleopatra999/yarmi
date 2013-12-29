
// Copyright (c) 2013, niXman (i dotty nixman doggy gmail dotty com)
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

#ifndef _yarmi__formatters__pair_hpp
#define _yarmi__formatters__pair_hpp

#include <iostream>
#include <utility>

#include <yarmi/formatters/decorators.hpp>
#include <yarmi/formatters/quoting.hpp>

/***************************************************************************/

namespace std {

template<typename T0, typename T1>
ostream& operator<< (ostream &s, const std::pair<T0, T1> &o) {
	s << yarmi::decorators::object_open_symbol << "\"first\":";
	quoting(s, o.first);
	s << yarmi::decorators::default_delimiter << "\"second\":";
	quoting(s, o.second);
	return s << yarmi::decorators::object_close_symbol;
}

template<typename T0, typename T1>
istream& operator>> (istream &s, std::pair<T0, T1> &o) {
	if ( s.get() != yarmi::decorators::object_open_symbol )
		throw std::runtime_error("object open symbol expected '{'");

	return s;
}

} // ns std

/***************************************************************************/

#endif // _yarmi__formatters__pair_hpp
