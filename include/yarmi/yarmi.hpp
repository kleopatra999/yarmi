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

#ifndef _yarmi__yarmi_hpp
#define _yarmi__yarmi_hpp

#include <yarmi/formatters/decorators.hpp>
#include <yarmi/formatters/quoting.hpp>
#include <yarmi/formatters/list.hpp>
#include <yarmi/formatters/map.hpp>
#include <yarmi/formatters/pair.hpp>
#include <yarmi/formatters/set.hpp>
#include <yarmi/formatters/tuple.hpp>
#include <yarmi/formatters/unordered_map.hpp>
#include <yarmi/formatters/unordered_set.hpp>
#include <yarmi/formatters/vector.hpp>

#include <boost/preprocessor.hpp>
#include <boost/type_traits/function_traits.hpp>

#include <cstdio>
#include <cstdint>
#include <string>
#include <stdexcept>

/***************************************************************************/

#if !defined(YARMI_USE_BINARY_SERIALIZATION) && \
	 !defined(YARMI_USE_TEXT_SERIALIZATION)
# define YARMI_USE_BINARY_SERIALIZATION (1)
//# define YARMI_USE_TEXT_SERIALIZATION (1)
#endif

#include <yas/mem_streams.hpp>

#if YARMI_USE_BINARY_SERIALIZATION
#	include <yas/binary_iarchive.hpp>
#	include <yas/binary_oarchive.hpp>
#	define YARMI_ISTREAM_TYPE yas::mem_istream
#	define YARMI_OSTREAM_TYPE yas::mem_ostream
#	define YARMI_IARCHIVE_TYPE yas::binary_iarchive<yas::mem_istream>
#	define YARMI_OARCHIVE_TYPE yas::binary_oarchive<yas::mem_ostream>
#elif YARMI_USE_TEXT_SERIALIZATION
#	include <yas/text_iarchive.hpp>
#	include <yas/text_oarchive.hpp>
#	define YARMI_IARCHIVE_TYPE yas::text_iarchive<yas::mem_istream>
#	define YARMI_OARCHIVE_TYPE yas::text_oarchive<yas::mem_ostream>
#endif

#include <yas/serializers/std_types_serializers.hpp>

/***************************************************************************/

#define YARMI_SYSTEM_ERROR_API_NAME \
	yarmi_error

#define YARMI_SYSTEM_ERROR_API_SIG \
	(std::uint8_t, std::uint8_t, std::string)

/***************************************************************************/

#define YARMI_DECLARE_MESSAGE_WRAP_X(...) \
	((__VA_ARGS__)) YARMI_DECLARE_MESSAGE_WRAP_Y
#define YARMI_DECLARE_MESSAGE_WRAP_Y(...) \
	((__VA_ARGS__)) YARMI_DECLARE_MESSAGE_WRAP_X
#define YARMI_DECLARE_MESSAGE_WRAP_X0
#define YARMI_DECLARE_MESSAGE_WRAP_Y0

/***************************************************************************/

#define YARMI_LAZY_IIF(bit, true_args, false_args, true_macro, false_macro) \
	YARMI_LAZY_IIF_I(bit, true_args, false_args, true_macro, false_macro)

#define YARMI_LAZY_IIF_I(bit, true_args, false_args, true_macro, false_macro) \
	YARMI_LAZY_IIF_ ## bit(true_args, false_args, true_macro, false_macro)

#define YARMI_LAZY_IIF_0(true_args, false_args, true_macro, false_macro) \
	false_macro false_args

#define YARMI_LAZY_IIF_1(true_args, false_args, true_macro, false_macro) \
	true_macro true_args

#define YARMI_LAZY_IF(cond, true_args, false_args, true_macro, false_macro) \
	YARMI_LAZY_IIF(BOOST_PP_BOOL(cond), true_args, false_args, true_macro, false_macro)

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

#define YARMI_COMMA_IF_NOT_LAST_ITERATION(size, idx) \
	BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(idx, BOOST_PP_SUB(size, 1)))

/***************************************************************************/

#define YARMI_PROCEDURE_INVOKING_FOR_EMPTY_ARGS(message_name) \
	impl.on_##message_name();

#define YARMI_PROCEDURE_INVOKING_MEMBERS(unused, idx, tuple) \
	BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_SIZE(tuple), idx, tuple) arg##idx;

#define YARMI_PROCEDURE_INVOKING_MEMBERS_DESERIALIZER(unused, idx, tuple) \
	& arg##idx

#define YARMI_PROCEDURE_INVOKING_FOR_NONEMPTY_ARGS(message_name, tuple) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_TUPLE_SIZE(tuple) \
		,YARMI_PROCEDURE_INVOKING_MEMBERS \
		,tuple \
	) \
	ia \
		BOOST_PP_REPEAT( \
			 BOOST_PP_TUPLE_SIZE(tuple) \
			,YARMI_PROCEDURE_INVOKING_MEMBERS_DESERIALIZER \
			,tuple \
		) \
	; \
	impl.on_##message_name(BOOST_PP_ENUM_PARAMS(BOOST_PP_TUPLE_SIZE(tuple), arg));

#define YARMI_CASES_FOR_CALL_VERSION_0(unused, idx, tuple) \
	YARMI_LAZY_IF( \
		YARMI_TUPLE_IS_EMPTY(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple))), \
		( \
			BOOST_PP_TUPLE_ELEM(2, 0, tuple) \
		), \
		( \
			BOOST_PP_TUPLE_ELEM(2, 0, tuple), \
			BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple)) \
		), \
		YARMI_PROCEDURE_INVOKING_FOR_EMPTY_ARGS, \
		YARMI_PROCEDURE_INVOKING_FOR_NONEMPTY_ARGS \
	)

#define YARMI_CASES_FOR_CALL_VERSION_1(unused, idx, tuple) \
	case idx: { \
		YARMI_LAZY_IF( \
		YARMI_TUPLE_IS_EMPTY(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple))), \
			( \
				BOOST_PP_TUPLE_ELEM(2, 0, tuple) \
			), \
			( \
				BOOST_PP_TUPLE_ELEM(2, 0, tuple), \
				BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple)) \
			), \
			YARMI_PROCEDURE_INVOKING_FOR_EMPTY_ARGS, \
			YARMI_PROCEDURE_INVOKING_FOR_NONEMPTY_ARGS \
		) \
	}; \
	break;

// generate cases for one version
#define YARMI_CASES_0(unused, idx, seq) \
	case idx: { \
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))) \
			,YARMI_CASES_FOR_CALL_VERSION_0 \
			,BOOST_PP_SEQ_ELEM(idx, seq) \
		) \
	}; \
	break;

// generate cases for multiply versions
#define YARMI_CASES_1(unused, idx, seq) \
	case idx: { \
		switch ( call_version ) { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))) \
				,YARMI_CASES_FOR_CALL_VERSION_1 \
				,BOOST_PP_SEQ_ELEM(idx, seq) \
			) \
		} \
	}; \
	break;

// if versions for this procedure more then one - call 'YARMI_CASES_1', otherwise 'YARMI_CASES_0'
#define YARMI_CASES(unused, idx, seq) \
	BOOST_PP_CAT( \
		 YARMI_CASES_ \
		,BOOST_PP_GREATER(BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))), 1) \
	)(unused, idx, seq)

/***************************************************************************/

#define YARMI_DECLARE_REMOTE_CALL_PARAMS(unused, idx, tuple) \
	const BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_SIZE(tuple), idx, tuple) &arg##idx \
		YARMI_COMMA_IF_NOT_LAST_ITERATION(BOOST_PP_TUPLE_SIZE(tuple), idx)


#define YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_EMPTY(name, idx, version) \
	void name() { \
		YARMI_OARCHIVE_TYPE oa(yas::no_header); \
		oa & static_cast<std::uint8_t>(idx) \
			& static_cast<std::uint8_t>(version) \
		; \
		YARMI_OARCHIVE_TYPE pa; \
		pa & oa.get_intrusive_buffer(); \
		io.send(pa.get_shared_buffer()); \
	}

#define YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY_SERIALIZE(unused, idx, data) \
	& arg##idx

#define YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY(name, idx, version, tuple) \
	void name( \
		BOOST_PP_REPEAT( \
			BOOST_PP_TUPLE_SIZE(tuple), \
			YARMI_DECLARE_REMOTE_CALL_PARAMS, \
			tuple \
		) \
	) { \
		YARMI_OSTREAM_TYPE os; \
		YARMI_OARCHIVE_TYPE oa(os, yas::no_header); \
		oa & static_cast<std::uint8_t>(idx) \
			& static_cast<std::uint8_t>(version) \
			BOOST_PP_REPEAT( \
				 BOOST_PP_TUPLE_SIZE(tuple) \
				,YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY_SERIALIZE \
				,~ \
			) \
		; \
		YARMI_OSTREAM_TYPE os2; \
		YARMI_OARCHIVE_TYPE pa(os2); \
		pa & os.get_intrusive_buffer(); \
		io.send(os2.get_shared_buffer()); \
	}

#define YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION(unused, idx, tuple) \
	YARMI_LAZY_IF( \
		YARMI_TUPLE_IS_EMPTY(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(3, 2, tuple))), \
		( \
			 BOOST_PP_TUPLE_ELEM(3, 0, tuple) \
			,BOOST_PP_TUPLE_ELEM(3, 1, tuple) \
			,idx \
		), \
		( \
			 BOOST_PP_TUPLE_ELEM(3, 0, tuple) \
			,BOOST_PP_TUPLE_ELEM(3, 1, tuple) \
			,idx \
			,BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(3, 2, tuple)) \
		), \
		YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_EMPTY, \
		YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY \
	)

#define YARMI_DECLARE_REMOTE_ONE_CALL(unused, idx, seq) \
	BOOST_PP_REPEAT( \
		BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))), \
		YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION, \
		( \
			 BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq)) \
			,idx \
			,BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq)) \
		) \
	)

#define YARMI_REMOTE_CALLS(seq) \
	BOOST_PP_REPEAT( \
		BOOST_PP_SEQ_SIZE(seq), \
		YARMI_DECLARE_REMOTE_ONE_CALL, \
		seq \
	)

/***************************************************************************/

#define YARMI_CALLS_VERSIONS_ONE_CALL(unused, idx, seq) \
	BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))), 1) \
		YARMI_COMMA_IF_NOT_LAST_ITERATION(BOOST_PP_SEQ_SIZE(seq), idx)

#define YARMI_CALLS_VERSIONS(seq) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_CALLS_VERSIONS_ONE_CALL \
		,seq \
	)

/***************************************************************************/

#define YARMI_PREPEND_SEQ(seq) \
	BOOST_PP_SEQ_PUSH_FRONT( \
		 seq \
		,(YARMI_SYSTEM_ERROR_API_NAME, (YARMI_SYSTEM_ERROR_API_SIG)) \
	)

/***************************************************************************/

#define YARMI_CALLS_NAMES_ONE_ONE_CALL(unused, idx, seq) \
	BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) \
		YARMI_COMMA_IF_NOT_LAST_ITERATION(BOOST_PP_SEQ_SIZE(seq), idx)

#define YARMI_CALLS_NAMES(seq) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_CALLS_NAMES_ONE_ONE_CALL \
		,seq \
	)

/***************************************************************************/

#define YARMI_INVOKER(classname, seq, opposeq) \
	template<typename Impl, typename IO = Impl> \
	struct classname { \
		classname(Impl &impl, IO &io) \
			:impl(impl) \
			,io(io) \
		{} \
		\
		YARMI_REMOTE_CALLS(YARMI_PREPEND_SEQ(opposeq)) \
		\
		void invoke(const char *ptr, std::size_t size) { \
			std::uint8_t call_id, call_version; \
			\
			static const char* names[] = { \
				YARMI_CALLS_NAMES(YARMI_PREPEND_SEQ(seq)) \
			}; \
			static const std::uint8_t versions[] = { \
				YARMI_CALLS_VERSIONS(YARMI_PREPEND_SEQ(seq)) \
			}; \
			\
			try { \
				YARMI_ISTREAM_TYPE is(ptr, size); \
				YARMI_IARCHIVE_TYPE ia(is, yas::no_header); \
				ia & call_id \
					& call_version; \
				\
				if ( call_id < 0 || call_id > BOOST_PP_SEQ_SIZE(seq) ) { \
					char errstr[1024] = {0}; \
					std::snprintf( \
						 errstr \
						,sizeof(errstr) \
						,"%s::%s(): bad call_id %d" \
						,#classname \
						,__FUNCTION__ \
						,static_cast<int>(call_id) \
					); \
					throw std::runtime_error(errstr); \
				} \
				if ( call_version > versions[call_id] ) { \
					char errstr[1024] = {0}; \
					std::snprintf( \
						 errstr \
						,sizeof(errstr) \
						,"%s::%s(): bad call_version %d for call_id %d(%s::%s())" \
						,#classname \
						,__FUNCTION__ \
						,static_cast<int>(call_version) \
						,static_cast<int>(call_id) \
						,#classname \
						,names[call_id] \
					); \
					throw std::runtime_error(errstr); \
				} \
				\
				switch ( call_id ) { \
					BOOST_PP_REPEAT( \
						 BOOST_PP_SEQ_SIZE(YARMI_PREPEND_SEQ(seq)) \
						,YARMI_CASES \
						,YARMI_PREPEND_SEQ(seq) \
					) \
				} \
			} catch (const std::exception &ex) { \
				char errstr[1024] = {0}; \
				std::snprintf( \
					 errstr \
					,sizeof(errstr) \
					,"std::exception is thrown when %s::%s() is called: '%s'" \
					,#classname \
					,names[call_id] \
					,ex.what() \
				); \
				YARMI_SYSTEM_ERROR_API_NAME(call_id, call_version, errstr); \
			} catch (...) { \
				char errstr[1024] = {0}; \
				std::snprintf( \
					 errstr \
					,sizeof(errstr) \
					,"unknown exception is thrown when %s::%s() is called" \
					,#classname \
					,names[call_id] \
				); \
				YARMI_SYSTEM_ERROR_API_NAME(call_id, call_version, errstr); \
			} \
		} \
		\
	private: \
		Impl &impl; \
		IO &io; \
	};

/***************************************************************************/

#define YARMI_CONSTRUCT( \
	client_invoker_name, \
	client_apis_seq, \
	server_invoker_name, \
	server_apis_seq \
) \
	namespace yarmi { \
		YARMI_INVOKER( \
			 client_invoker_name \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X client_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X server_apis_seq, 0) \
		) \
		YARMI_INVOKER( \
			 server_invoker_name \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X server_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X client_apis_seq, 0) \
		) \
	} // ns yarmi

/***************************************************************************/

#define YARMI_DECLARE_ENUM_MEMBERS(unused, idx, seq) \
	BOOST_PP_IF( \
		 BOOST_PP_EQUAL(2 ,BOOST_PP_TUPLE_SIZE(BOOST_PP_SEQ_ELEM(idx, seq))) \
		,BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))=BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq)) /* member = value */ \
		,BOOST_PP_TUPLE_ELEM(1, 0, BOOST_PP_SEQ_ELEM(idx, seq)) /* member */ \
	) /* BOOST_PP_IF */ \
	BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_ADD(idx, 1), BOOST_PP_SEQ_SIZE(seq)))

#define YARMI_DECLARE_ENUM_WRITE_CASES(unused, idx, tuple) \
	case BOOST_PP_TUPLE_ELEM(2, 0, tuple)::BOOST_PP_IF( \
		 BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_SIZE(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple)))) \
		,BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple))) \
		,BOOST_PP_TUPLE_ELEM(1, 0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple))) \
	) /* BOOST_PP_IF */ \
	: \
	return BOOST_PP_STRINGIZE( \
		BOOST_PP_TUPLE_ELEM(2, 0, tuple)::BOOST_PP_IF( \
			 BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_SIZE(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple)))) \
			,BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple))) \
			,BOOST_PP_TUPLE_ELEM(1, 0, BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, 1, tuple))) \
		) /* BOOST_PP_IF */ \
	) \
	;

#define YARMI_DECLARE_ENUM_IMPL(name, type, spec, seq) \
	enum class name: type { \
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(seq) \
			,YARMI_DECLARE_ENUM_MEMBERS \
			,seq \
		) \
	}; \
	\
	spec const char* enum_cast(const name &o) { \
		switch (o) { \
			BOOST_PP_REPEAT( \
				BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_DECLARE_ENUM_WRITE_CASES \
				,(name, seq) \
			) \
		} \
		return #name "::unknown"; \
	} \
	\
	spec std::ostream& operator<< (std::ostream &s, const name &o) { \
		return s << enum_cast(o); \
	}

#define YARMI_DECLARE_ENUM(name, type, spec, seq) \
	YARMI_DECLARE_ENUM_IMPL( \
		 name \
		,type \
		,spec \
		,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X seq, 0) \
	)

/***************************************************************************/

#include <iostream>

template<typename T>
void read_value(std::istream &s, const char *name, T &o) {
	if ( s.get() != '\"' )
		throw std::runtime_error("bad JSON: expected '\"' before key name");
	std::string key;
	std::getline(s, key, '\"');
	if ( key != name )
		throw std::runtime_error("bad JSON: expected key \"" + std::string(name) + "\"");
	//std::cout << "key:\"" << key << "\", expected:\"" << name << "\"" << std::endl;
	if ( s.get() != ':' )
		throw std::runtime_error("bad JSON: expected ':' before data-member");
	s >> o;
}

#if 0
if ( s.get() != '\"' ) \
	throw std::runtime_error("bad JSON: expected '\"' before key name"); \
std::string BOOST_PP_CAT(s, BOOST_PP_CAT(idx, BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq)))); \
std::getline(s, BOOST_PP_CAT(s, BOOST_PP_CAT(idx, BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq)))), '\"'); \
if ( \
	BOOST_PP_CAT(s, BOOST_PP_CAT(idx, BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq)))) != \
	BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) \
) throw std::runtime_error( \
	"bad JSON: expected key \"" \
	BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) \
	"\"" \
); \
std::cout << "key:" << BOOST_PP_CAT(s, BOOST_PP_CAT(idx, BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq)))) << std::endl; \
if ( s.get() != ':' ) \
	throw std::runtime_error("bad JSON: expected ':' before data-member");
#endif

#define YARMI_DECLARE_MESSAGE_DECLARE_MEMBER(unused, idx, seq) \
	boost::function_traits<void BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))>::arg1_type \
		BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq));

#define YARMI_DECLARE_MESSAGE_ENUMERATE_SERIALIZED(unused, idx, seq) \
	BOOST_PP_IF(BOOST_PP_LESS(idx, BOOST_PP_SEQ_SIZE(seq)), &, ) \
		BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))

#define YARMI_DECLARE_MESSAGE_ENUMERATE_OUTPUTED(unused, idx, seq) \
	s << BOOST_PP_IF(BOOST_PP_EQUAL(idx, 0), , ",") \
		"\"" BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) "\":"; \
	quoting(s, o.BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq)));

#define YARMI_DECLARE_MESSAGE_ENUMERATE_INPUTED(unused, idx, seq) \
	std::cout << idx << std::endl; \
	read_value( \
		 s \
		,BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) \
		,o.BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq)) \
	);

#define YARMI_DECLARE_MESSAGE_MEMBER_NAME(unused, idx, seq) \
	BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(idx, seq))) \
		BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_ADD(idx, 1), BOOST_PP_SEQ_SIZE(seq)))

#define YARMI_DECLARE_MESSAGE_ENUMERATE_MEMBERS(unused, data, elem) \
	data(BOOST_PP_TUPLE_ELEM(2, 0, elem));

#define YARMI_DECLARE_MESSAGE_ENUMERATE_TYPES(unused, idx, seq) \
	BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))) \
		BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_ADD(idx, 1), BOOST_PP_SEQ_SIZE(seq)))

/***************************************************************************/

#define YARMI_DECLARE_MESSAGE_IMPL(name, seq, ...) \
	struct name { \
	private: \
		static constexpr const char* __meta_names[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_DECLARE_MESSAGE_MEMBER_NAME \
				,seq \
			) \
		}; \
		static constexpr const char* __meta_types[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				 ,YARMI_DECLARE_MESSAGE_ENUMERATE_TYPES \
				,seq \
			) \
		}; \
		\
	public: \
		static constexpr const char* meta_name() { return #name; } \
		static constexpr std::size_t meta_count() { return BOOST_PP_SEQ_SIZE(seq); } \
		\
		static constexpr const char* const* meta_members() { return __meta_names; } \
		static constexpr const char* meta_member(std::size_t idx) { \
			return (idx < sizeof(__meta_names)/sizeof(__meta_names[0]) ? __meta_names[idx] : 0); \
		} \
		static constexpr const char* const* meta_types() { return __meta_types; } \
		static constexpr const char* meta_type(std::size_t idx) { \
			return (idx < sizeof(__meta_types)/sizeof(__meta_types[0]) ? __meta_types[idx] : 0); \
		} \
		\
		/* some code expanded here */ \
		__VA_ARGS__ \
		\
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(seq) \
			,YARMI_DECLARE_MESSAGE_DECLARE_MEMBER \
			,seq \
		) \
		\
		template<typename F> \
		void apply(const F &func_) const { \
			BOOST_PP_SEQ_FOR_EACH( \
				 YARMI_DECLARE_MESSAGE_ENUMERATE_MEMBERS \
				,func_ \
				,seq \
			) \
		} \
		template<typename F> \
		void transform(F &func_) { \
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
		friend std::ostream& operator<< (std::ostream &s, const name &o) { \
			s << '{'; \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_DECLARE_MESSAGE_ENUMERATE_OUTPUTED \
				,seq \
			) \
			s << '}'; \
			return s; \
		} \
		friend std::istream& operator>> (std::istream &s, name &o) { \
			if ( s.get() != '{' ) \
				throw std::runtime_error("bad JSON: \'{\'"); \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(seq) \
					,YARMI_DECLARE_MESSAGE_ENUMERATE_INPUTED \
					,seq \
				) \
			return s; \
		} \
	};

#define YARMI_DECLARE_MESSAGE( \
	 name /* type name */ \
	,seq /* types sequence */ \
	, ... /* some code */ \
) \
	YARMI_DECLARE_MESSAGE_IMPL( \
		 name \
		,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X seq, 0) \
		,__VA_ARGS__ \
	)

/***************************************************************************/

#endif // _yarmi__yarmi_hpp
