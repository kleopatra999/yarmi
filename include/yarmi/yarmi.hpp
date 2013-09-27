
#ifndef _yarmi__yarmi_hpp
#define _yarmi__yarmi_hpp

#include <boost/preprocessor.hpp>

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

#if YARMI_USE_BINARY_SERIALIZATION
#	include <yas/binary_iarchive.hpp>
#	include <yas/binary_oarchive.hpp>
#	define YARMI_IARCHIVE_TYPE yas::binary_mem_iarchive
#	define YARMI_OARCHIVE_TYPE yas::binary_mem_oarchive
#elif YARMI_USE_TEXT_SERIALIZATION
#	include <yas/text_iarchive.hpp>
#	include <yas/text_oarchive.hpp>
#	define YARMI_IARCHIVE_TYPE yas::text_mem_iarchive
#	define YARMI_OARCHIVE_TYPE yas::text_mem_oarchive
#endif

#include <yas/serializers/std_types_serializers.hpp>

/***************************************************************************/

#define YARMI_SYSTEM_ERROR_API_NAME \
	yarmi_error

#define YARMI_SYSTEM_ERROR_API_SIG \
	(std::uint8_t, std::uint8_t, std::string)

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
	impl->on_##message_name();

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
	impl->on_##message_name(BOOST_PP_ENUM_PARAMS(BOOST_PP_TUPLE_SIZE(tuple), arg));

#define YARMI_CASES_FOR_CALL_VERSION(unused, idx, tuple) \
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

#define YARMI_CASES(unused, idx, seq) \
	case idx: { \
		switch ( call_version ) { \
			BOOST_PP_REPEAT( \
				BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(idx, seq))), \
				YARMI_CASES_FOR_CALL_VERSION, \
				BOOST_PP_SEQ_ELEM(idx, seq) \
			) \
		} \
	}; \
	break;

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
		const yas::shared_buffer buffer = pa.get_shared_buffer(); \
		io->send(buffer.data, buffer.size); \
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
		YARMI_OARCHIVE_TYPE oa(yas::no_header); \
		oa & static_cast<std::uint8_t>(idx) \
			& static_cast<std::uint8_t>(version) \
			BOOST_PP_REPEAT( \
				 BOOST_PP_TUPLE_SIZE(tuple) \
				,YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY_SERIALIZE \
				,~ \
			) \
		; \
		YARMI_OARCHIVE_TYPE pa; \
		pa & oa.get_intrusive_buffer(); \
		io->send(pa.get_shared_buffer()); \
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

#define YARMI_APPEND_SEQ(seq) \
	BOOST_PP_SEQ_PUSH_BACK( \
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
		classname(Impl *impl, IO *io) \
			:impl(impl) \
			,io(io) \
		{} \
		\
		YARMI_REMOTE_CALLS(YARMI_APPEND_SEQ(opposeq)) \
		\
		void invoke(const char *ptr, std::size_t size) { \
			std::uint8_t call_id, call_version; \
			\
			static const char* names[] = { \
				YARMI_CALLS_NAMES(YARMI_APPEND_SEQ(seq)) \
			}; \
			static const std::uint8_t versions[] = { \
				YARMI_CALLS_VERSIONS(YARMI_APPEND_SEQ(seq)) \
			}; \
			\
			try { \
				YARMI_IARCHIVE_TYPE ia(ptr, size, yas::no_header); \
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
						 BOOST_PP_SEQ_SIZE(YARMI_APPEND_SEQ(seq)) \
						,YARMI_CASES \
						,YARMI_APPEND_SEQ(seq) \
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
		Impl *impl; \
		IO *io; \
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
			,client_apis_seq \
			,server_apis_seq \
		) \
		YARMI_INVOKER( \
			 server_invoker_name \
			,server_apis_seq \
			,client_apis_seq \
		) \
	} // ns yarmi

/***************************************************************************/

#endif // _yarmi__yarmi_hpp
