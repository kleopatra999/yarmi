
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

#include <yarmi/fnv1a.hpp>
#include <yarmi/serialization.hpp>
#include <yarmi/generate_ns.hpp>
#include <yarmi/generate_lazy_if.hpp>
#include <yarmi/generate_tuple_is_empty.hpp>
#include <yarmi/generate_ns_to_string.hpp>
#include <yarmi/generate_metacode.hpp>
#include <yarmi/generate_callers.hpp>
#include <yarmi/generate_invoker.hpp>
#include <yarmi/generate_invokers_sfinae.hpp>
#include <yarmi/generate_struct.hpp>
#include <yarmi/generate_enum.hpp>
#include <yarmi/generate_proc_helper.hpp>

/***************************************************************************/

namespace yarmi {

template<typename Invoker, typename... Invokers>
bool invoke(const char *ptr, const std::size_t size, call_id_type *cid, Invoker &head, Invokers&... tail) {
	istream_type istream(ptr, size);
	iarchive_type iarchive(istream, yas::no_header);
	call_id_type call_id = 0;
	iarchive & call_id;
	if ( cid ) *cid = call_id;

	bool flag  = false;
	auto apply = [](...) {};
	auto func  = [&flag](const call_id_type call_id, iarchive_type &iarchive, Invoker &invoker) {
		return flag=flag || invoker.invoke(call_id, iarchive);
	};
	apply(func(call_id, iarchive, head), func(call_id, iarchive, tail)...);

	return flag;
}

} // ns yarmi

/***************************************************************************/

#define YARMI_WRAP_SEQUENCE_X(...) \
	((__VA_ARGS__)) YARMI_WRAP_SEQUENCE_Y
#define YARMI_WRAP_SEQUENCE_Y(...) \
	((__VA_ARGS__)) YARMI_WRAP_SEQUENCE_X

#define YARMI_WRAP_SEQUENCE_X0
#define YARMI_WRAP_SEQUENCE_Y0

/***************************************************************************/

#define YARMI_COMMA_IF_NOT_LAST_ITERATION(size, idx) \
	BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(idx, BOOST_PP_SUB(size, 1)))

/***************************************************************************/

#define YARMI_CONSTRUCT_INVOKER( \
	 ns      /* namespace */ \
	,cn      /* class name */ \
	,oppons  /* namespace for opposite class */ \
	,oppocn  /* name of opposite class */ \
	,seq     /* procedures sequence */ \
	,opposeq /* opposite procedures sequence */ \
) \
	template<typename Impl, typename IO = Impl> \
	struct cn { \
		using this_type = cn<Impl, IO>; \
		\
		cn(Impl &impl, IO &io) \
			:impl(impl) \
			,io(io) \
		{} \
		\
		YARMI_GENERATE_METACODE(ns, cn, oppons, oppocn, seq, opposeq) \
		YARMI_GENERATE_CALLERS(ns, oppocn, seq) \
		YARMI_GENERATE_INVOKERS(opposeq) \
	\
	private: \
		Impl &impl; \
		IO &io; \
	\
	private: \
		YARMI_GENERATE_INVOKERS_SFINAE(opposeq) \
	}; \
	\
	template<typename Impl, typename IO> \
	constexpr const char* cn<Impl, IO>::_meta_requests_names[]; \
	template<typename Impl, typename IO> \
	constexpr const char* cn<Impl, IO>::_meta_handlers_names[];

/***************************************************************************/

#define YARMI_CONSTRUCT( \
	 client_invoker_ns \
	,client_invoker_name \
	,client_apis_seq \
	,server_invoker_ns \
	,server_invoker_name \
	,server_apis_seq \
	,... \
) \
	namespace YARMI_GENERATE_NS_NAME(client_invoker_ns, server_invoker_ns) \
	{ \
		__VA_ARGS__; \
	} /* ns */ \
	\
	YARMI_GENERATE_OPEN_NS(client_invoker_ns) \
		using namespace YARMI_GENERATE_NS_NAME(client_invoker_ns, server_invoker_ns); \
		\
		YARMI_CONSTRUCT_INVOKER( \
			 client_invoker_ns \
			,client_invoker_name \
			,server_invoker_ns \
			,server_invoker_name \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X client_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X server_apis_seq, 0) \
		) \
	YARMI_GENERATE_CLOSE_NS(client_invoker_ns) \
	\
	YARMI_GENERATE_OPEN_NS(server_invoker_ns) \
		using namespace YARMI_GENERATE_NS_NAME(client_invoker_ns, server_invoker_ns); \
		\
		YARMI_CONSTRUCT_INVOKER( \
			 server_invoker_ns \
			,server_invoker_name \
			,client_invoker_ns \
			,client_invoker_name \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X server_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_WRAP_SEQUENCE_X client_apis_seq, 0) \
		) \
	YARMI_GENERATE_CLOSE_NS(server_invoker_ns)

/***************************************************************************/

#endif // _yarmi__yarmi_hpp
