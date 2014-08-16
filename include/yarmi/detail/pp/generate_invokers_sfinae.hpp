
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

#ifndef _yarmi__detail__pp__generate_invokers_sfinae_hpp
#define _yarmi__detail__pp__generate_invokers_sfinae_hpp

#define YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name) \
	BOOST_PP_CAT(_, BOOST_PP_CAT(name, BOOST_PP_CAT(_, idx)))

#define YARMI_GENERATE_INVOKERS_SFINAE_IMPL(idx, name, _tuple) \
	template<typename Obj, typename... Args> \
	static auto YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name) \
		(Obj &o, const Args&... args) -> decltype(o.name(args..., std::make_tuple(std::cref(args)...)), void()) { \
			o.name(args..., std::make_tuple(std::cref(args)...)); \
	} \
	template<typename Obj, typename... Args> \
	static auto YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name) \
		(Obj &o, const Args&... args) -> decltype(o.name(args...), void()) { \
			o.name(args...); \
	}

#define YARMI_GENERATE_INVOKERS_SFINAE_AUX(unused, idx, seq) \
	YARMI_GENERATE_INVOKERS_SFINAE_IMPL( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(idx, seq)) \
		,BOOST_PP_TUPLE_ELEM(2, BOOST_PP_SEQ_ELEM(idx, seq)) \
	)

#define YARMI_GENERATE_INVOKERS_SFINAE(seq) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_GENERATE_INVOKERS_SFINAE_AUX \
		,seq \
	)

#endif // _yarmi__detail__pp__generate_invokers_sfinae_hpp
