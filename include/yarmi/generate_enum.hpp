
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

#ifndef _yarmi__generate_enum_hpp
#define _yarmi__generate_enum_hpp

/***************************************************************************/

#define YARMI_GENERATE_ENUM_MEMBERS(unused, idx, seq) \
	BOOST_PP_IF( \
		 BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_SIZE(BOOST_PP_SEQ_ELEM(idx, seq))) \
		,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq))=BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(idx, seq)) /* member = value */ \
		,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq)) /* member */ \
	) \
		BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_ADD(idx, 1), BOOST_PP_SEQ_SIZE(seq)))

#define YARMI_GENERATE_ENUM_WRITE_CASES(unused, idx, tuple) \
	case BOOST_PP_TUPLE_ELEM(0, tuple)::BOOST_PP_IF( \
			 BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_SIZE(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple)))) \
			,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple))) \
			,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple))) \
		) /* BOOST_PP_IF */ \
		: \
		return "\"" BOOST_PP_STRINGIZE( \
			BOOST_PP_TUPLE_ELEM(0, tuple)::BOOST_PP_IF( \
				 BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_SIZE(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple)))) \
				,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple))) \
				,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple))) \
			) /* BOOST_PP_IF */ \
		) "\"" \
	;

/***************************************************************************/

#define YARMI_GENERATE_ENUM_CLASS_GENERATE_OPERATORS_ELEM(unused, idx, tuple) \
	inline BOOST_PP_TUPLE_ELEM(1, tuple) operator BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) ( \
		 const BOOST_PP_TUPLE_ELEM(1, tuple) &l \
		,const BOOST_PP_TUPLE_ELEM(1, tuple) &r \
	) { \
		return static_cast<BOOST_PP_TUPLE_ELEM(1, tuple)>( \
			static_cast<BOOST_PP_TUPLE_ELEM(2, tuple)>(l) \
			BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) \
			static_cast<BOOST_PP_TUPLE_ELEM(2, tuple)>(r)) \
		; \
	}

#define YARMI_GENERATE_ENUM_CLASS_GENERATE_OPERATORS(name, type, seq) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_GENERATE_ENUM_CLASS_GENERATE_OPERATORS_ELEM \
		,(seq, name, type) \
	)

/***************************************************************************/

#define YARMI_GENERATE_ENUM_CLASS_IMPL(name, type, seq) \
	enum class name: type { \
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(seq) \
			,YARMI_GENERATE_ENUM_MEMBERS \
			,seq \
		) \
	}; \
	\
	inline const char* enum_cast(const name &e) { \
		switch (e) { \
			BOOST_PP_REPEAT( \
				BOOST_PP_SEQ_SIZE(seq), \
				YARMI_GENERATE_ENUM_WRITE_CASES, \
				(name, seq) \
			) \
		} \
		return "\"" #name "::unknown\""; \
	} \
	\
	inline std::ostream& operator<< (std::ostream &s, const name &e) { \
		return s << enum_cast(e); \
	} \
	YARMI_GENERATE_ENUM_CLASS_GENERATE_OPERATORS(name, type, (&)(|)(^))

#define YARMI_GENERATE_ENUM_CLASS(name, type, seq) \
	YARMI_GENERATE_ENUM_CLASS_IMPL( \
		 name \
		,type \
		,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X seq, 0) \
	)

/***************************************************************************/

#define YARMI_GENERATE_ENUM_IMPL(name, seq) \
	enum name { \
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(seq) \
			,YARMI_GENERATE_ENUM_MEMBERS \
			,seq \
		) \
	}; \
	\
	inline const char* enum_cast(const name &e) { \
		switch (e) { \
			BOOST_PP_REPEAT( \
				BOOST_PP_SEQ_SIZE(seq), \
				YARMI_GENERATE_ENUM_WRITE_CASES, \
				(name, seq) \
			) \
		} \
		return "\"" #name "::unknown\""; \
	} \
	\
	inline std::ostream& operator<< (std::ostream &s, const name &e) { \
		return s << enum_cast(e); \
	}

#define YARMI_GENERATE_ENUM(name, seq) \
	YARMI_GENERATE_ENUM_IMPL( \
		 name \
		,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X seq, 0) \
	)

/***************************************************************************/

#endif // _yarmi__generate_enum_hpp
