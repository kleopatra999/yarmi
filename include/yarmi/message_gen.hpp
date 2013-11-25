
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

#ifndef _yarmi__message_gen_hpp
#define _yarmi__message_gen_hpp

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>

#include <boost/type_traits/function_traits.hpp>

/***************************************************************************/

#define YARMI_DECLARE_MESSAGE_DECLARE_MEMBER(unused, idx, seq) \
	boost::function_traits<void BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))>::arg1_type \
		BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq));

#define YARMI_DECLARE_MESSAGE_ENUMERATE_SERIALIZED(unused, idx, seq) \
	BOOST_PP_IF(BOOST_PP_LESS(idx, BOOST_PP_SEQ_SIZE(seq)), &, ) \
		BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))

#define YARMI_DECLARE_MESSAGE_ENUMERATE_OUTPUTED(unused, idx, seq) \
	s << BOOST_PP_IF(BOOST_PP_EQUAL(idx, 0), , ", ") \
		"\"" BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) "\":"; \
	quoting(s, o.BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))); \

#define YARMI_DECLARE_MESSAGE_MEMBER_NAME(unused, idx, seq) \
	BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) \
		BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_ADD(idx, 1), BOOST_PP_SEQ_SIZE(seq)))

#define YARMI_DECLARE_MESSAGE_ENUMERATE_MEMBERS(unused, data, elem) \
	data(BOOST_PP_TUPLE_ELEM(2, 0, elem));

#define YARMI_DECLARE_MESSAGE_ENUMERATE_TYPES(unused, idx, seq) \
	BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))) \
		BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_ADD(idx, 1), BOOST_PP_SEQ_SIZE(seq)))

/***************************************************************************/

#define YARMI_DECLARE_MESSAGE(tname, seq, ...) \
	struct tname { \
	private: \
		static constexpr const char* _names[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_DECLARE_MESSAGE_MEMBER_NAME \
				,seq \
			) \
		}; \
		static constexpr const char* _types[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				 ,YARMI_DECLARE_MESSAGE_ENUMERATE_TYPES \
				,seq \
			) \
		}; \
		\
	public: \
		static constexpr const char* meta_name() { return #tname; } \
		static constexpr std::size_t meta_count() { return BOOST_PP_SEQ_SIZE(seq); } \
		static constexpr const char* const* meta_members() { return _names; } \
		static constexpr const char* meta_type(std::size_t idx) { \
			return (idx < sizeof(_types)/sizeof(_types[0]) ? _types[idx] : 0); \
		} \
		\
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(seq) \
			,YARMI_DECLARE_MESSAGE_DECLARE_MEMBER \
			,seq \
		) \
		\
		template<typename F> \
		void apply(F func_) const { \
			BOOST_PP_SEQ_FOR_EACH( \
				 YARMI_DECLARE_MESSAGE_ENUMERATE_MEMBERS \
				,func_ \
				,seq \
			) \
		} \
		template<typename F> \
		void transform(F func_) { \
			BOOST_PP_SEQ_FOR_EACH( \
				 YARMI_DECLARE_MESSAGE_ENUMERATE_MEMBERS \
				,func_ \
				,seq \
			) \
		} \
		\
		template<typename Archive> \
		void serialize(Archive &ar) { \
			ar \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(seq) \
					,YARMI_DECLARE_MESSAGE_ENUMERATE_SERIALIZED \
					,seq \
				) \
			; \
		} \
		\
		friend std::ostream& operator<< (std::ostream &s, const tname &o) { \
			s << '{'; \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_DECLARE_MESSAGE_ENUMERATE_OUTPUTED \
				,seq \
			) \
			s << '}'; \
			return s; \
		} \
		/* user code here */ \
		__VA_ARGS__ \
	};

/***************************************************************************/

#endif // _yarmi__message_gen_hpp
