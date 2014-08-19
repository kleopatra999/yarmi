
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

#ifndef _yarmi__yarmi_hpp
#define _yarmi__yarmi_hpp

#include <boost/preprocessor.hpp>

#include <yarmi/yarmi_fwd.hpp>

#include <yarmi/detail/pp/generate_tools.hpp>
#include <yarmi/detail/pp/generate_ns.hpp>
#include <yarmi/detail/pp/generate_lazy_if.hpp>
#include <yarmi/detail/pp/generate_tuple_is_empty.hpp>
#include <yarmi/detail/pp/generate_ns_to_string.hpp>
#include <yarmi/detail/pp/generate_metacode.hpp>
#include <yarmi/detail/pp/generate_callers.hpp>
#include <yarmi/detail/pp/generate_invoker.hpp>
#include <yarmi/detail/pp/generate_invokers_sfinae.hpp>
#include <yarmi/detail/pp/generate_proc_helper.hpp>

#include <yarmi/detail/throw/throw.hpp>

#include <yarmi/generate_struct.hpp>
#include <yarmi/generate_enum.hpp>

/***************************************************************************/

#define YARMI_CONSTRUCT_INVOKER( \
	 ser		/* serializer name */ \
	,ns		/* namespace */ \
	,cn		/* class name */ \
	,oppocn	/* name of opposite class */ \
	,seq		/* procedures sequence */ \
	,opposeq	/* opposite procedures sequence */ \
) \
	template< \
		 typename Impl \
		,typename IO = Impl \
		,typename CLog = ::yarmi::_fake_log \
		,typename ILog = ::yarmi::_fake_log \
	> \
	struct cn { \
		using serializer = ser; \
		\
		cn( \
			 Impl &impl \
			,IO &io \
			,CLog clog = ::yarmi::fake_log \
			,ILog ilog = ::yarmi::fake_log \
		) \
			:impl(impl) \
			,io(io) \
			,clog(clog) \
			,ilog(ilog) \
		{} \
		\
		YARMI_GENERATE_METACODE(ns, cn, oppocn, seq, opposeq) \
		YARMI_GENERATE_CALLERS(ns, oppocn, seq) \
		YARMI_GENERATE_INVOKERS(opposeq) \
		\
	private: \
		Impl &impl; \
		IO &io; \
		CLog clog; \
		ILog ilog; \
		\
	private: \
		YARMI_GENERATE_INVOKERS_SFINAE(opposeq) \
	}; \
	\
	template<typename Impl, typename IO, typename CLog, typename ILog> \
	constexpr const char* cn<Impl, IO, CLog, ILog>::_meta_requests_names[]; \
	template<typename Impl, typename IO, typename CLog, typename ILog> \
	constexpr const char* cn<Impl, IO, CLog, ILog>::_meta_handlers_names[];

/***************************************************************************/

#define YARMI_CONSTRUCT( \
	 invokers_ns \
	,serializer \
	,client_invoker_name \
	,client_apis_seq \
	,server_invoker_name \
	,server_apis_seq \
	,... \
) \
	YARMI_GENERATE_OPEN_NS(invokers_ns) \
		__VA_ARGS__ /* user code expanded here */ \
		\
		YARMI_CONSTRUCT_INVOKER( \
			 serializer \
			,invokers_ns \
			,client_invoker_name \
			,server_invoker_name \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X client_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X server_apis_seq, 0) \
		) \
		\
		YARMI_CONSTRUCT_INVOKER( \
			 serializer \
			,invokers_ns \
			,server_invoker_name \
			,client_invoker_name \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X server_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X client_apis_seq, 0) \
		) \
	YARMI_GENERATE_CLOSE_NS(invokers_ns)

/***************************************************************************/

#endif // _yarmi__yarmi_hpp
