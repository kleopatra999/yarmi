
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

#include <yas/mem_streams.hpp>
#include <yas/serializers/std_types_serializers.hpp>

#include <boost/preprocessor.hpp>

#include <yarmi/declare_enum.hpp>
#include <yarmi/declare_ns.hpp>
#include <yarmi/declare_lazy_if.hpp>
#include <yarmi/declare_tuple_is_empty.hpp>
#include <yarmi/declare_ns_to_string.hpp>
#include <yarmi/declare_get_api_name.hpp>
#include <yarmi/declare_invoker.hpp>
#include <yarmi/declare_callers.hpp>
#include <yarmi/declare_has_handler.hpp>

#include <cstdint>

/***************************************************************************/

#if !defined(YARMI_USE_BINARY_SERIALIZATION) && \
	 !defined(YARMI_USE_TEXT_SERIALIZATION)
# define YARMI_USE_BINARY_SERIALIZATION (1)
//# define YARMI_USE_TEXT_SERIALIZATION (1)
#endif

#if YARMI_USE_BINARY_SERIALIZATION
#	include <yas/binary_iarchive.hpp>
#	include <yas/binary_oarchive.hpp>
#	define YARMI_ISTREAM_TYPE yas::mem_istream
#	define YARMI_OSTREAM_TYPE yas::mem_ostream
#	define YARMI_IARCHIVE_TYPE yas::binary_iarchive<YARMI_ISTREAM_TYPE>
#	define YARMI_OARCHIVE_TYPE yas::binary_oarchive<YARMI_OSTREAM_TYPE>
#elif YARMI_USE_TEXT_SERIALIZATION
#	include <yas/text_iarchive.hpp>
#	include <yas/text_oarchive.hpp>
#	define YARMI_ISTREAM_TYPE yas::mem_istream
#	define YARMI_OSTREAM_TYPE yas::mem_ostream
#	define YARMI_IARCHIVE_TYPE yas::text_iarchive<YARMI_ISTREAM_TYPE>
#	define YARMI_OARCHIVE_TYPE yas::text_oarchive<YARMI_OSTREAM_TYPE>
#endif

/***************************************************************************/

#define YARMI_COMMA_IF_NOT_LAST_ITERATION(size, idx) \
	BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(idx, BOOST_PP_SUB(size, 1)))

/***************************************************************************/

#define YARMI_CONSTRUCT_INVOKER(ns, cn, oppocn, seq, opposeq) \
	template<typename Impl, typename IO = Impl> \
	struct cn { \
		private: \
			template<std::uint32_t N> \
			static constexpr std::uint32_t fnv1a_32(const char(&s)[N], std::uint32_t i=0, std::uint32_t h=0x811c9dc5) { \
				return (i==N-1)?h:fnv1a_32(s, i+1, ((h^s[i])*0x01000193)); \
			} \
		public: \
		\
		cn(Impl &impl, IO &io) \
			:impl(impl) \
			,io(io) \
		{} \
		\
		YARMI_GENERATE_GET_API_NAME(ns, cn, opposeq) \
		YARMI_GENERATE_HAS_HANDLER(ns, cn, opposeq) \
		\
		YARMI_GENERATE_CALLERS(ns, oppocn, seq) \
		\
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
	YARMI_CONSTRUCT_INVOKER_OPEN_NS(client_invoker_ns) \
		YARMI_CONSTRUCT_INVOKER( \
			 client_invoker_ns \
			,client_invoker_name \
			,server_invoker_name \
			,client_apis_seq \
			,server_apis_seq \
		) \
	YARMI_CONSTRUCT_INVOKER_CLOSE_NS(client_invoker_ns) \
	\
	YARMI_CONSTRUCT_INVOKER_OPEN_NS(server_invoker_ns) \
		YARMI_CONSTRUCT_INVOKER( \
			 server_invoker_ns \
			,server_invoker_name \
			,client_invoker_name \
			,server_apis_seq \
			,client_apis_seq \
		) \
	YARMI_CONSTRUCT_INVOKER_CLOSE_NS(server_invoker_ns)

/***************************************************************************/

#endif // _yarmi__yarmi_hpp
