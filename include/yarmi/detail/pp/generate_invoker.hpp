
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

#ifndef _yarmi__detail__pp__generate_invoker_hpp
#define _yarmi__detail__pp__generate_invoker_hpp

/***************************************************************************/

#define YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_EMPTY_ARGS(idx, name) \
	const auto packet_args = std::make_tuple(call_id, call_name); \
	YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name)( \
		 impl \
		,packet_args \
		,std::make_index_sequence<0>() \
	);

#define YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_MEMBERS(unused, idx, tuple) \
	BOOST_PP_TUPLE_ELEM(idx, tuple) arg##idx;

#define YARMI_GENERATE_INVOKERS_GENERATE_ENUM_ARGS_FOR_TUPLE_PLACE_ELEM(unused1, idx, size) \
	std::cref(arg##idx) YARMI_COMMA_IF_NOT_LAST_ITERATION(size, idx)

#define YARMI_GENERATE_INVOKERS_GENERATE_ENUM_ARGS_FOR_TUPLE(tuple) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_TUPLE_SIZE(tuple) \
		,YARMI_GENERATE_INVOKERS_GENERATE_ENUM_ARGS_FOR_TUPLE_PLACE_ELEM \
		,BOOST_PP_TUPLE_SIZE(tuple) \
	)

#define YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_NONEMPTY_ARGS(idx, name, tuple) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_TUPLE_SIZE(tuple) \
		,YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_MEMBERS \
		,tuple \
	) \
	Serializer::deserialize(buffer, BOOST_PP_ENUM_PARAMS(BOOST_PP_TUPLE_SIZE(tuple), arg)); \
	const auto packet_args = std::make_tuple( \
			 call_id \
			,call_name \
			,YARMI_GENERATE_INVOKERS_GENERATE_ENUM_ARGS_FOR_TUPLE(tuple) \
		); \
	YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name)( \
		 impl\
		,packet_args \
		,std::make_index_sequence<BOOST_PP_TUPLE_SIZE(tuple)>() \
	);

/***************************************************************************/

#define YARMI_GENERATE_INVOKERS_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_INVOKERS_GET_PROC_NAME_, str)

#define YARMI_GENERATE_INVOKERS_GET_PROC_ARGS(str) \
	BOOST_PP_CAT(YARMI_GENERATE_INVOKERS_GET_PROC_ARGS_, str)

#define YARMI_GENERATE_INVOKERS_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_INVOKERS_GEN_proc

#define YARMI_GENERATE_INVOKERS_GET_PROC_NAME_enum(...) \
	YARMI_GENERATE_INVOKERS_GEN_enum

#define YARMI_GENERATE_INVOKERS_GET_PROC_NAME_struct(...) \
	YARMI_GENERATE_INVOKERS_GEN_struct

#define YARMI_GENERATE_INVOKERS_GET_PROC_ARGS_proc(request, handler, tuple) \
	(handler, tuple)

#define YARMI_GENERATE_INVOKERS_GET_PROC_ARGS_enum(...) \
	__VA_ARGS__

#define YARMI_GENERATE_INVOKERS_GET_PROC_ARGS_struct(...) \
	__VA_ARGS__

#define YARMI_GENERATE_INVOKERS_GEN_proc(idx, name, tuple) \
	case BOOST_PP_CAT(_handler_id_, BOOST_PP_CAT(name, _##idx)): { \
		YARMI_LAZY_IF( \
			 YARMI_TUPLE_IS_EMPTY(tuple) \
			,(idx, name) \
			,(idx, name, tuple) \
			,YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_EMPTY_ARGS \
			,YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_NONEMPTY_ARGS \
		) \
		return true; \
	}

#define YARMI_GENERATE_INVOKERS_GEN_enum(...)

#define YARMI_GENERATE_INVOKERS_GEN_struct(...)

#define YARMI_GENERATE_INVOKERS_EXPAND_MACRO(idx, pname, tuple) \
	pname( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(0, tuple) \
		,BOOST_PP_TUPLE_ELEM(1, tuple) \
	)

#define YARMI_GENERATE_INVOKERS_AUX(unused, idx, seq) \
	YARMI_GENERATE_INVOKERS_EXPAND_MACRO( \
		 idx \
		,YARMI_GENERATE_INVOKERS_GET_PROC_NAME BOOST_PP_SEQ_ELEM(idx, seq) \
		,YARMI_GENERATE_INVOKERS_GET_PROC_ARGS BOOST_PP_SEQ_ELEM(idx, seq) \
	)

#define YARMI_GENERATE_INVOKERS(seq) \
	bool invoke(const ::yarmi::call_id_type call_id, const ::yarmi::buffer_pair &buffer) override { \
		::yarmi::call_name_type call_name = meta_handler_name(call_id); \
		switch ( call_id ) { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_INVOKERS_AUX \
				,seq \
			) \
			default: return false; \
		} \
	}

/***************************************************************************/

#endif // _yarmi__detail__pp__generate_invoker_hpp
