
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

#ifndef _yarmigen__tokens_hpp
#define _yarmigen__tokens_hpp

namespace yarmigen {

/***************************************************************************/

#define YARMIGEN_DECL_C(name, val) \
	static constexpr const char name = val

#define YARMIGEN_DECL_S(name, val) \
	static constexpr const char name[] = val

YARMIGEN_DECL_S(proto_str, "proto");

YARMIGEN_DECL_C(proto_str_open_char, '(');
YARMIGEN_DECL_C(proto_str_close_char, ')');
YARMIGEN_DECL_C(proto_body_open_char, '{');
YARMIGEN_DECL_C(proto_body_close_char, '}');
YARMIGEN_DECL_C(proto_body_close_dotcomma_char, ';');

YARMIGEN_DECL_S(type_api_str, "api");
YARMIGEN_DECL_S(type_service_str, "service");

YARMIGEN_DECL_S(namespace_str, "namespace");
YARMIGEN_DECL_C(namespace_close_dotcomma_char, ';');

YARMIGEN_DECL_S(class_str, "class");
YARMIGEN_DECL_C(class_close_dotcomma_char, ';');

YARMIGEN_DECL_C(section_open_char, '[');
YARMIGEN_DECL_C(section_closechar, ']');

YARMIGEN_DECL_C(enum_body_open_char, '{');
YARMIGEN_DECL_C(enum_body_close_char, '}');
YARMIGEN_DECL_C(enum_body_close_dotcomma_char, ';');
YARMIGEN_DECL_C(enum_type_separator, ':');
YARMIGEN_DECL_C(enum_assign_char, '=');

YARMIGEN_DECL_C(using_assign_char, '=');
YARMIGEN_DECL_C(using_close_dotcomma_char, ';');

YARMIGEN_DECL_C(const_assign_char, '=');
YARMIGEN_DECL_C(const_close_dotcomma_char, ';');

YARMIGEN_DECL_C(struct_body_open_char, '{');
YARMIGEN_DECL_C(struct_body_close_char, '}');
YARMIGEN_DECL_C(struct_body_close_dotcomma_char, ';');
YARMIGEN_DECL_C(struct_base_classes_separator, ':');

YARMIGEN_DECL_C(proc_sig_open_char, '(');
YARMIGEN_DECL_C(proc_sig_close_char, ')');
YARMIGEN_DECL_C(proc_sig_close_dotcomma_char, ';');

/***************************************************************************/

} // ns yarmigen

#endif // _yarmigen__tokens_hpp
