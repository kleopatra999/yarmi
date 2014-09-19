
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

#ifndef _yarmi__detail__pp__generate_metacode_hpp
#define _yarmi__detail__pp__generate_metacode_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/integral_c.hpp>

/***************************************************************************/

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_NAME_, str)

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_ARGS(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_ARGS_, str)

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GEN_proc

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_NAME_enum(...) \
	YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GEN_enum

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_NAME_struct(...) \
	YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GEN_struct

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_ARGS_proc(request, handler, tuple) \
	(request, handler)

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_ARGS_enum(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_ARGS_struct(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GEN_proc(pref, idx, reqhand, name, names_arr) \
	static constexpr const ::yarmi::call_id_type \
		BOOST_PP_CAT(pref, BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(reqhand, name), _##idx)) \
			= ::yarmi::hash(names_arr[idx]);

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GEN_enum(...)

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GEN_struct(...)

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_EXPAND_MACRO(pref, idx, reqhand, pname, names_arr, name) \
	pname(pref, idx, reqhand, name, names_arr)
//[pref|idx|pname|names_arr|name]

#define YARMI_GENERATE_METACODE_GENERATE_CALL_IDS(unused, idx, tuple) \
	YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_EXPAND_MACRO( \
		 BOOST_PP_TUPLE_ELEM(1, tuple) \
		,idx \
		,BOOST_PP_TUPLE_ELEM(3, tuple) \
		,YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_NAME BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) \
		,BOOST_PP_TUPLE_ELEM(2, tuple) \
		,YARMI_GENERATE_METACODE_GENERATE_CALL_IDS_GET_PROC_ARGS BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_NAME_, str)

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_ARGS(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_ARGS_, str)

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GEN_proc

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_NAME_enum(...) \
	YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GEN_enum

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_NAME_struct(...) \
	YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GEN_struct

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_ARGS_proc(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_ARGS_enum(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_ARGS_struct(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GEN_proc(ns, cn, tuple) \
	YARMI_GENERATE_NS_TO_STRING(ns, cn::BOOST_PP_TUPLE_ELEM(1, (tuple))BOOST_PP_TUPLE_ELEM(2, (tuple))) \
	,

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GEN_enum(...) \
	"",

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GEN_struct(...) \
	"",

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_EXPAND_MACRO(ns, cn, name, args) \
	name(ns, cn, args)

#define YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS(unused, idx, tuple) \
	YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_EXPAND_MACRO( \
		 BOOST_PP_TUPLE_ELEM(0, tuple) \
		,BOOST_PP_TUPLE_ELEM(1, tuple) \
		,YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_NAME BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, tuple)) \
		,YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS_GET_PROC_ARGS BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, tuple)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_NAME_, str)

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_ARGS(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_ARGS_, str)

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GEN_proc

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_NAME_enum(...) \
	YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GEN_enum

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_NAME_struct(...) \
	YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GEN_struct

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_ARGS_proc(request, handler, tuple) \
	(request, handler)

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_ARGS_enum(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_ARGS_struct(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GEN_proc(pref, idx, reqhand, size, tuple) \
	::boost::mpl::integral_c<::yarmi::call_id_type, BOOST_PP_CAT(pref, BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(reqhand, tuple), _##idx))> \
		YARMI_COMMA_IF_NOT_LAST_ITERATION(size, idx)

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GEN_enum(...)

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GEN_struct(...)

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_EXPAND_MACRO(pref, idx, reqhand, size, pname, tuple) \
	pname(pref, idx, reqhand, size, tuple)

#define YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR(unused, idx, tuple) \
	YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_EXPAND_MACRO( \
		 BOOST_PP_TUPLE_ELEM(1, tuple) \
		,idx \
		,BOOST_PP_TUPLE_ELEM(2, tuple) \
		,BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(0, tuple)) \
		,YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_NAME BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) \
		,YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR_GET_PROC_ARGS BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_NAME_, str)

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_ARGS(str) \
	BOOST_PP_CAT(YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_ARGS_, str)

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GEN_proc

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_NAME_enum(...) \
	YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GEN_enum

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_NAME_struct(...) \
	YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GEN_struct

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_ARGS_proc(request, handler, tuple) \
	(request, handler)

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_ARGS_enum(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_ARGS_struct(...) \
	__VA_ARGS__

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GEN_proc(pref, idx, name, names_arr) \
	call_id == BOOST_PP_CAT(pref, BOOST_PP_CAT(name, _##idx)) ? names_arr[idx] :

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GEN_enum(pref, idx, ...)

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GEN_struct(pref, idx, ...)

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_EXPAND_MACRO(pref, idx, reqhand, names_arr, pname, tuple) \
	pname(pref, idx, BOOST_PP_TUPLE_ELEM(reqhand, tuple), names_arr)

#define YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY(unused, idx, tuple) \
	YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_EXPAND_MACRO( \
		 BOOST_PP_TUPLE_ELEM(1, tuple) \
		,idx \
		,BOOST_PP_TUPLE_ELEM(3, tuple) \
		,BOOST_PP_TUPLE_ELEM(2, tuple) \
		,YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_NAME BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) \
		,YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY_GET_PROC_ARGS BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(0, tuple)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE(ns, cn, oppocn, seq, opposeq) \
	protected: \
		static constexpr const char *_meta_requests_names[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS \
				,(ns, oppocn, seq) \
			) \
			nullptr \
		}; \
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(seq) \
			,YARMI_GENERATE_METACODE_GENERATE_CALL_IDS \
			,(seq, _request_id_, _meta_requests_names, 0 /* requests */ ) \
		) \
		\
		static constexpr const char *_meta_handlers_names[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(opposeq) \
				,YARMI_GENERATE_METACODE_GENERATE_PROC_STRINGS \
				,(ns, cn, opposeq) \
			) \
			nullptr \
		}; \
		BOOST_PP_REPEAT( \
			 BOOST_PP_SEQ_SIZE(opposeq) \
			,YARMI_GENERATE_METACODE_GENERATE_CALL_IDS \
			,(opposeq, _handler_id_, _meta_handlers_names, 1 /* handlers */ ) \
		) \
		\
		static void dump(std::ostream &os, const char *const *list) { \
			os << "invoker: \"" YARMI_GENERATE_NS_TO_STRING(ns, cn) "\"" << std::endl; \
			for ( ; *list; ++list ) { \
				os << "  " << *list << ": 0x" << std::hex << ::yarmi::hash(*list) << std::endl; \
			} \
		} \
		\
	public: \
		using _meta_requests_ids_vec = ::boost::mpl::vector< \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR \
				,(seq, _request_id_, 0 /* requests */ ) \
			) \
		>; \
		\
		using _meta_handlers_ids_vec = ::boost::mpl::vector< \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(opposeq) \
				,YARMI_GENERATE_METACODE_GENERATE_IDS_MPL_VECTOR \
				,(opposeq, _handler_id_, 1 /* handlers */ ) \
			) \
		>; \
		\
		static constexpr const char* const* meta_requests() { return _meta_requests_names; } \
		static constexpr std::size_t  meta_requests_count() { return (sizeof(_meta_requests_names)/sizeof(_meta_requests_names[0]))-1; } \
		static constexpr const char*  meta_request_name(const ::yarmi::call_id_type call_id) { \
			return ( \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(seq) \
					,YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY \
					,(seq, _request_id_, _meta_requests_names, 0 /* requests */ ) \
				) \
				nullptr \
			); \
		} \
		static constexpr bool meta_has_request(const ::yarmi::call_id_type call_id) { return meta_request_name(call_id) != nullptr; } \
		static constexpr bool meta_has_request(const char *str) { return meta_has_request(::yarmi::hash(str)); } \
		static void dump_requests(std::ostream &os) { dump(os, meta_requests()); } \
		\
		static constexpr const char* const* meta_handlers() { return _meta_handlers_names; } \
		static constexpr std::size_t  meta_handlers_count() { return (sizeof(_meta_handlers_names)/sizeof(_meta_handlers_names[0]))-1; } \
		static constexpr const char*  meta_handler_name(const ::yarmi::call_id_type call_id) { \
			return ( \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(opposeq) \
					,YARMI_GENERATE_METACODE_GENERATE_HAS_PROC_TERNARY \
					,(opposeq, _handler_id_, _meta_handlers_names, 1 /* handlers */ ) \
				) \
				nullptr \
			); \
		} \
		static constexpr bool meta_has_handler(const ::yarmi::call_id_type call_id) { return meta_handler_name(call_id) != nullptr; } \
		static constexpr bool meta_has_handler(const char *str) { return meta_has_handler(::yarmi::hash(str)); } \
		static void dump_handlers(std::ostream &os) { dump(os, meta_handlers()); }

/***************************************************************************/

#endif // _yarmi__detail__pp__generate_metacode_hpp
