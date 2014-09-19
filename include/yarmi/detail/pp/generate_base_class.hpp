
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

#ifndef _yarmi__detail__pp__generate_base_class_hpp
#define _yarmi__detail__pp__generate_base_class_hpp

/***************************************************************************/

#define YARMI_GENERATE_BASE_CLASS_NAME(name) \
	BOOST_PP_CAT(name, _base)

/***************************************************************************/

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_NAME_, str)

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_ARGS(str) \
	BOOST_PP_CAT(YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_ARGS_, str)

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GEN_proc

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_NAME_enum(...) \
	YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GEN_enum

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_NAME_struct(...) \
	YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GEN_struct

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_ARGS_proc(request, handler, tuple) \
	(request, handler)

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_ARGS_enum(...) \
	__VA_ARGS__

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_ARGS_struct(...) \
	__VA_ARGS__

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GEN_proc(cn, pref, idx, name) \
	template<typename T> \
	constexpr const ::yarmi::call_id_type cn<T>::BOOST_PP_CAT(pref, BOOST_PP_CAT(name, _##idx));

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GEN_enum(...)

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GEN_struct(...)

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_EXPAND_MACRO(cn, pref, idx, reqhand, pname, tuple) \
	pname(cn, pref, idx, BOOST_PP_TUPLE_ELEM(reqhand, tuple))

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_IMPL(unused, idx, tuple) \
	YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_EXPAND_MACRO( \
		 BOOST_PP_TUPLE_ELEM(0, tuple) \
		,BOOST_PP_TUPLE_ELEM(2, tuple) \
		,idx \
		,BOOST_PP_TUPLE_ELEM(3, tuple) \
		,YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_NAME BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple)) \
		,YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_GET_PROC_ARGS BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(1, tuple)) \
	)

#define YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT(seq, opposeq, cn) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_IMPL \
		,(cn, seq, _request_id_, 0 /* requests */ ) \
	) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(opposeq) \
		,YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT_IMPL \
		,(cn, opposeq, _handler_id_, 1 /* handlers */ ) \
	)

/***************************************************************************/

#define YARMI_GENERATE_BASE_CLASS(ns, cn, oppocn, seq, opposeq) \
	template<typename T> \
	struct YARMI_GENERATE_BASE_CLASS_NAME(cn): ::yarmi::invoker_base { \
		YARMI_GENERATE_METACODE(ns, cn, oppocn, seq, opposeq) \
	}; \
	template<typename T> \
	constexpr const char* YARMI_GENERATE_BASE_CLASS_NAME(cn)<T>::_meta_requests_names[]; \
	template<typename T> \
	constexpr const char* YARMI_GENERATE_BASE_CLASS_NAME(cn)<T>::_meta_handlers_names[]; \
	\
	YARMI_GENERATE_BASE_CLASS_GENERATE_PROC_IDS_EXTERN_INIT( \
		 seq \
		,opposeq \
		,YARMI_GENERATE_BASE_CLASS_NAME(cn) \
	)

/***************************************************************************/

#endif // _yarmi__detail__pp__generate_base_class_hpp
