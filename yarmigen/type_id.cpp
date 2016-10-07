
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

#include "type_id.hpp"

#include <algorithm>
#include <string>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

namespace yarmigen {

/***************************************************************************/

#define YARMIGEN_DECLARE_TYPES_MAP_IMPL(unused1, data, elem) \
	{	 BOOST_PP_STRINGIZE(elem) \
		,BOOST_PP_TUPLE_ELEM(0, data)::BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(1, data), elem) \
	},

#define YARMIGEN_DECLARE_TYPES_MAP(name, enum, pref, seq) \
	static const pair name[] = { \
		BOOST_PP_SEQ_FOR_EACH( \
			 YARMIGEN_DECLARE_TYPES_MAP_IMPL \
			,(enum, pref) \
			,seq \
		) \
		{nullptr, enum::BOOST_PP_CAT(pref, unknown)} \
	};

/***************************************************************************/

struct pair {
	const char *name;
	const type_id type;
};

YARMIGEN_DECLARE_TYPES_MAP(
	 map
	,YARMIGEN_TYPE_ID_ENUM_NAME
	,YARMIGEN_TYPES_ENUM_PREFIX
	,YARMIGEN_TYPES_ENUM_SEQ
)

static const auto beg = &map[0];
static const auto end = &map[sizeof(map)/sizeof(pair)];

/***************************************************************************/

type_id type_id_by_name(const std::string &name) {
	const auto it = std::find_if(
		 beg, end
		,[&name](const pair &p) { return p.name != nullptr && p.name == name; }
	);

	if ( it != end )
		return it->type;

	return type_id::type_unknown;
}

const char* type_name_by_id(const type_id id) {
	const auto it = std::find_if(
		 beg, end
		,[id](const pair &p) { return p.name != nullptr && p.type == id; }
	);

	if ( it != end )
		return it->name;

	return nullptr;
}

/***************************************************************************/

bool is_type_name(const std::string &name) {
	// template type decl
	const auto p = name.find('<');
	return type_id_by_name((p != std::string::npos ? name.substr(0, p) : name)) != type_id::type_unknown;
}

/***************************************************************************/

} // ns yarmigen
