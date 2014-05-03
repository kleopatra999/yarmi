
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
#include <yarmi/generate_enum.hpp>
#include <yarmi/generate_ns.hpp>
#include <yarmi/generate_lazy_if.hpp>
#include <yarmi/generate_tuple_is_empty.hpp>
#include <yarmi/generate_ns_to_string.hpp>
#include <yarmi/generate_metacode.hpp>
#include <yarmi/generate_callers.hpp>
#include <yarmi/generate_invoker.hpp>

/***************************************************************************/

#ifndef YARMI_GENERATE_INVOKE_NUM_INVOKERS
#	define YARMI_GENERATE_INVOKE_NUM_INVOKERS 10
#endif // YARMI_GENERATE_INVOKE_NUM_INVOKERS

/***************************************************************************/

#define YARMI_DECLARE_MESSAGE_WRAP_X(...) \
	((__VA_ARGS__)) YARMI_DECLARE_MESSAGE_WRAP_Y
#define YARMI_DECLARE_MESSAGE_WRAP_Y(...) \
	((__VA_ARGS__)) YARMI_DECLARE_MESSAGE_WRAP_X

#define YARMI_DECLARE_MESSAGE_WRAP_X0
#define YARMI_DECLARE_MESSAGE_WRAP_Y0

/***************************************************************************/

#define YARMI_COMMA_IF_NOT_LAST_ITERATION(size, idx) \
	BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(idx, BOOST_PP_SUB(size, 1)))

/***************************************************************************/

#define YARMI_CONSTRUCT_INVOKER(ns, cn, oppons, oppocn, seq, opposeq) \
	template<typename Impl, typename IO = Impl> \
	struct cn { \
		cn(Impl &impl, IO &io) \
			:impl(impl) \
			,io(io) \
		{} \
		\
		YARMI_GENERATE_METACODE(ns, cn, oppons, oppocn, seq, opposeq) \
		\
		YARMI_GENERATE_CALLERS(ns, oppocn, seq) \
		YARMI_GENERATE_INVOKERS(ns, cn, opposeq) \
		\
	private: \
		Impl &impl; \
		IO &io; \
	};

/***************************************************************************/

#define YARMI_CONSTRUCT( \
	 client_invoker_ns \
	,client_invoker_name \
	,client_apis_seq \
	,server_invoker_ns \
	,server_invoker_name \
	,server_apis_seq \
) \
	YARMI_OPEN_NS(client_invoker_ns) \
		YARMI_CONSTRUCT_INVOKER( \
			 client_invoker_ns \
			,client_invoker_name \
			,server_invoker_ns \
			,server_invoker_name \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X client_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X server_apis_seq, 0) \
		) \
	YARMI_CLOSE_NS(client_invoker_ns) \
	\
	YARMI_OPEN_NS(server_invoker_ns) \
		YARMI_CONSTRUCT_INVOKER( \
			 server_invoker_ns \
			,server_invoker_name \
			,client_invoker_ns \
			,client_invoker_name \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X server_apis_seq, 0) \
			,BOOST_PP_CAT(YARMI_DECLARE_MESSAGE_WRAP_X client_apis_seq, 0) \
		) \
	YARMI_CLOSE_NS(server_invoker_ns)

/***************************************************************************/

namespace yarmi {

/***************************************************************************/

#define YARMI_GENERATE_ONE_INVOKE_REPEATED_OR(unused1, idx, size) \
	inv##idx.invoke(call_id, iarchive) BOOST_PP_IF(BOOST_PP_LESS(BOOST_PP_ADD(idx, 1), size), ||, )

#define YARMI_GENERATE_ONE_INVOKE(unused1, idx, unused2) \
	template<BOOST_PP_ENUM_PARAMS(idx, typename Inv)> \
	bool invoke(const char *ptr, const std::size_t size, id_type *cid, BOOST_PP_ENUM_BINARY_PARAMS(idx, Inv, &inv)) { \
		istream_type istream(ptr, size); \
		iarchive_type iarchive(istream, yas::no_header); \
		id_type call_id = 0; \
		iarchive & call_id; \
		if ( cid ) *cid = call_id; \
		return ( \
			BOOST_PP_REPEAT( \
				 idx \
				,YARMI_GENERATE_ONE_INVOKE_REPEATED_OR \
				,idx \
			) \
		); \
	}

#define YARMI_GENERATE_INVOKE(num) \
	template<typename Inv0> \
	bool invoke(const char *ptr, const std::size_t size, id_type *cid, Inv0 &inv0) { \
		istream_type istream(ptr, size); \
		iarchive_type iarchive(istream, yas::no_header); \
		id_type call_id = 0; \
		iarchive & call_id; \
		if ( cid ) *cid = call_id; \
		return inv0.invoke(call_id, iarchive); \
	} \
	\
	BOOST_PP_REPEAT_FROM_TO( \
		 2 \
		,BOOST_PP_ADD(num, 1) \
		,YARMI_GENERATE_ONE_INVOKE \
		,~ \
	)

YARMI_GENERATE_INVOKE(YARMI_GENERATE_INVOKE_NUM_INVOKERS)

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__yarmi_hpp
