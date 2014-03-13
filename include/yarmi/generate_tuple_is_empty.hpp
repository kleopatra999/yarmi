
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

#ifndef _yarmi__declare_tuple_is_empty_hpp
#define _yarmi__declare_tuple_is_empty_hpp

/***************************************************************************/
// based on the: http://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments

#define YARMI_ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define YARMI_HAS_COMMA(...) YARMI_ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define YARMI__TRIGGER_PARENTHESIS_(...) ,

#define YARMI_ISEMPTY(...) \
	YARMI__ISEMPTY( \
		/* test if there is just one argument, eventually an empty one */ \
		YARMI_HAS_COMMA(__VA_ARGS__), \
		/* test if _TRIGGER_PARENTHESIS_ together with the argument adds a comma */ \
		YARMI_HAS_COMMA(YARMI__TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
		/* test if the argument together with a parenthesis adds a comma */ \
		YARMI_HAS_COMMA(__VA_ARGS__ (/*empty*/)), \
		/* test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */ \
		YARMI_HAS_COMMA(YARMI__TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/)) \
	)

#define YARMI_PASTE5(_0, _1, _2, _3, _4) \
	_0 ## _1 ## _2 ## _3 ## _4

#define YARMI__ISEMPTY(_0, _1, _2, _3) \
	YARMI_HAS_COMMA(YARMI_PASTE5(YARMI__IS_EMPTY_CASE_, _0, _1, _2, _3))

#define YARMI__IS_EMPTY_CASE_0001 \
	,

#define YARMI_TUPLE_TO_ARGS(...) \
	__VA_ARGS__

#define YARMI_TUPLE_IS_EMPTY(tuple) \
	YARMI_ISEMPTY(YARMI_TUPLE_TO_ARGS tuple)

/***************************************************************************/

#endif // _yarmi__declare_tuple_is_empty_hpp
