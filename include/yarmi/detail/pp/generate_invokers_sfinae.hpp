
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

#ifndef _yarmi__detail__pp__generate_invokers_sfinae_hpp
#define _yarmi__detail__pp__generate_invokers_sfinae_hpp

/***************************************************************************/

#define YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name) \
	_##name##_##idx

/***************************************************************************/

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_NAME_, str)

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_ARGS(str) \
	BOOST_PP_CAT(YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_ARGS_, str)

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_INVOKERS_SFINAE_GEN_proc

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_NAME_enum(...) \
	YARMI_GENERATE_INVOKERS_SFINAE_GEN_enum

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_NAME_struct(...) \
	YARMI_GENERATE_INVOKERS_SFINAE_GEN_struct

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_ARGS_proc(request, handler, tuple) \
	handler

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_ARGS_enum(...)

#define YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_ARGS_struct(...)

#define YARMI_GENERATE_INVOKERS_SFINAE_GEN_proc(idx, handler) \
	template<typename Obj, typename Tuple, std::size_t... Ids> \
	static auto YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, handler) \
		(Obj &o, const Tuple &t, std::index_sequence<Ids...>) \
		-> decltype(o.handler(t), void()) \
	{ o.handler(t); } \
	\
	template<typename Obj, typename Tuple, std::size_t... Ids> \
	static auto YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, handler) \
		(Obj &o, const Tuple &t, std::index_sequence<Ids...>) \
		-> decltype(o.handler(std::get<Ids+2>(t)..., t), void()) \
	{ o.handler(std::get<Ids+2>(t)..., t); } \
	\
	template<typename Obj, typename Tuple, std::size_t... Ids> \
	static auto YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, handler) \
		(Obj &o, const Tuple &t, std::index_sequence<Ids...>) \
		-> decltype(o.handler(std::get<Ids+2>(t)..., std::get<0>(t), std::get<1>(t)), void()) \
	{ o.handler(std::get<Ids+2>(t)..., std::get<0>(t), std::get<1>(t)); } \
	\
	template<typename Obj, typename Tuple, std::size_t... Ids> \
	static auto YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, handler) \
		(Obj &o, const Tuple &t, std::index_sequence<Ids...>) \
		-> decltype(o.handler(std::get<Ids+2>(t)...), void()) \
	{ o.handler(std::get<Ids+2>(t)...); }

#define YARMI_GENERATE_INVOKERS_SFINAE_GEN_enum(...)

#define YARMI_GENERATE_INVOKERS_SFINAE_GEN_struct(...)

#define YARMI_GENERATE_INVOKERS_SFINAE_EXPAND_MACRO(idx, pname, handler) \
	pname(idx, handler)

#define YARMI_GENERATE_INVOKERS_SFINAE_AUX(unused, idx, seq) \
	YARMI_GENERATE_INVOKERS_SFINAE_EXPAND_MACRO( \
		 idx \
		,YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_NAME BOOST_PP_SEQ_ELEM(idx, seq) \
		,YARMI_GENERATE_INVOKERS_SFINAE_GET_PROC_ARGS BOOST_PP_SEQ_ELEM(idx, seq) \
	)

#define YARMI_GENERATE_INVOKERS_SFINAE(seq) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_GENERATE_INVOKERS_SFINAE_AUX \
		,seq \
	)

/***************************************************************************/

#endif // _yarmi__detail__pp__generate_invokers_sfinae_hpp
