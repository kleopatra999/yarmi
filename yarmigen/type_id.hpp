
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

#ifndef _yarmigen__type_id_hpp
#define _yarmigen__type_id_hpp

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <string>

namespace yarmigen {

/***************************************************************************/

#define YARMIGEN_TYPES_ENUM_SEQ \
	(bool) \
	(i8) \
	(u8) \
	(i16) \
	(u16) \
	(i32) \
	(u32) \
	(i64) \
	(u64) \
	(double) \
	(binary) \
	(string) \
	(array) \
	(set) \
	(map) \
	(unknown)

#define YARMIGEN_TYPE_ID_ENUM_NAME \
	type_id
#define YARMIGEN_TYPES_ENUM_PREFIX \
	type_

#define YARMIGEN_DECLARE_TYPES_ENUM_ELEM(unused1, pref, elem) \
	BOOST_PP_CAT(pref, elem),

#define YARMIGEN_DECLARE_TYPES_ENUM(name, pref, seq) \
	enum class name { \
		BOOST_PP_SEQ_FOR_EACH( \
			 YARMIGEN_DECLARE_TYPES_ENUM_ELEM \
			,pref \
			,seq \
		) \
	};

YARMIGEN_DECLARE_TYPES_ENUM(
	 YARMIGEN_TYPE_ID_ENUM_NAME
	,YARMIGEN_TYPES_ENUM_PREFIX
	,YARMIGEN_TYPES_ENUM_SEQ
)

/***************************************************************************/

type_id type_id_by_name(const std::string &name);
const char* type_name_by_id(const type_id id);
bool is_type_name(const std::string &name);

/***************************************************************************/

} // ns yarmigen

#endif // _yarmigen__type_id_hpp
