
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

#ifndef _yarmi__generate_struct_hpp
#define _yarmi__generate_struct_hpp

#include <yarmi/detail/generate_struct/decorators.hpp>
#include <yarmi/detail/generate_struct/list.hpp>
#include <yarmi/detail/generate_struct/map.hpp>
#include <yarmi/detail/generate_struct/pair.hpp>
#include <yarmi/detail/generate_struct/jsonify.hpp>
#include <yarmi/detail/generate_struct/set.hpp>
#include <yarmi/detail/generate_struct/tuple.hpp>
#include <yarmi/detail/generate_struct/unordered_map.hpp>
#include <yarmi/detail/generate_struct/unordered_set.hpp>
#include <yarmi/detail/generate_struct/vector.hpp>

/***************************************************************************/

#define YARMI_GENERATE_STRUCT_DECLARE_MEMBER(unused, idx, seq) \
	BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_POP_FRONT(BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_SEQ_ELEM(idx, seq)))) /* type */ \
		BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq)); /* var */

#define YARMI_GENERATE_STRUCT_ENUMERATE_SERIALIZED(unused, idx, seq) \
	BOOST_PP_IF(BOOST_PP_LESS(idx, BOOST_PP_SEQ_SIZE(seq)), &, ) \
		BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq))

#define YARMI_GENERATE_STRUCT_ENUMERATE_OUTPUTED(unused, idx, seq) \
	BOOST_PP_IF(BOOST_PP_EQUAL(idx, 0), , s << ",") \
		"\"" BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq))) "\":"; \
	::yarmi::detail::jsonify(s, o.BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq)));

#define YARMI_GENERATE_STRUCT_ENUMERATE_MEMBERS(unused, idx, seq) \
	func(BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq)));

/***************************************************************************/

#define YARMI_GENERATE_STRUCT_IMPL(struct_name, seq) \
	struct struct_name { \
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(seq) \
			,YARMI_GENERATE_STRUCT_DECLARE_MEMBER \
			,seq \
		) \
		\
		template<typename Archive> \
		void serialize(Archive &ar) { \
			ar \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(seq) \
					,YARMI_GENERATE_STRUCT_ENUMERATE_SERIALIZED \
					,seq \
				) \
			; \
		} \
		\
		template<typename F> \
		void apply(F func) const { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_STRUCT_ENUMERATE_MEMBERS \
				,seq \
			) \
		} \
		template<typename F> \
		void transform(F func) { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_STRUCT_ENUMERATE_MEMBERS \
				,seq \
			) \
		} \
		\
		friend std::ostream& operator<< (std::ostream &s, const struct_name &o) { \
			s << "{" \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_STRUCT_ENUMERATE_OUTPUTED \
				,seq \
			) \
			s << "}"; \
			return s; \
		} \
		\
		std::string to_string() const { \
			std::ostringstream os; \
			os << *this; \
			return os.str(); \
		} \
	};

#define YARMI_GENERATE_STRUCT( \
	 struct_name /* struct  name */ \
	,seq /* types sequence */ \
) \
	YARMI_GENERATE_STRUCT_IMPL( \
		 struct_name \
		,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X seq, 0) \
	)

/***************************************************************************/

#endif // _yarmi__generate_struct_hpp
