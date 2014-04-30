
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

#ifndef _yarmi__generate_metacode_hpp
#define _yarmi__generate_metacode_hpp

/***************************************************************************/

#define YARMI_GENERATE_METACODE_GET_ID_VAR_NAME(idx, tuple) \
	BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(1, tuple), _##idx)

#define YARMI_GENERATE_METACODE_REQUESTS_ENUM_IMPL(idx, name) \
	BOOST_PP_CAT(name, _##idx) = ::yarmi::detail::fnv1a(_meta_requests_names[idx]) \
	,

#define YARMI_GENERATE_METACODE_REQUESTS_ENUM_AUX(unused, idx, seq) \
	YARMI_GENERATE_METACODE_REQUESTS_ENUM_IMPL( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq)) \
	)

#define YARMI_GENERATE_METACODE_HANDLERS_ENUM_IMPL(idx, name) \
	BOOST_PP_CAT(name, _##idx) = ::yarmi::detail::fnv1a(_meta_handlers_names[idx]) \
	,

#define YARMI_GENERATE_METACODE_HANDLERS_ENUM_AUX(unused, idx, seq) \
	YARMI_GENERATE_METACODE_HANDLERS_ENUM_IMPL( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(idx, seq)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE_HANDLERS_NAMES_IMPL(ns, cn, tuple) \
	YARMI_NS_TO_STRING(ns, cn::BOOST_PP_TUPLE_ELEM(1, tuple) BOOST_PP_TUPLE_ELEM(2, tuple)) \
	,

#define YARMI_GENERATE_METACODE_HANDLERS_NAMES_AUX(unused, idx, tuple) \
	YARMI_GENERATE_METACODE_HANDLERS_NAMES_IMPL( \
		 BOOST_PP_TUPLE_ELEM(0, tuple) \
		,BOOST_PP_TUPLE_ELEM(1, tuple) \
		,BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, tuple)) \
	)

#define YARMI_GENERATE_METACODE_REQUESTS_NAMES_IMPL(ns, cn, tuple) \
	YARMI_NS_TO_STRING(ns, cn::BOOST_PP_TUPLE_ELEM(1, tuple) BOOST_PP_TUPLE_ELEM(2, tuple)) \
	,

#define YARMI_GENERATE_METACODE_REQUESTS_NAMES_AUX(unused, idx, tuple) \
	YARMI_GENERATE_METACODE_REQUESTS_NAMES_IMPL( \
		 BOOST_PP_TUPLE_ELEM(0, tuple) \
		,BOOST_PP_TUPLE_ELEM(1, tuple) \
		,BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, tuple)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE_HAS_REQUEST_IMPL(idx, name) \
	BOOST_PP_IF(idx,:,) call_id == static_cast<id_type>(_meta_requests_ids::BOOST_PP_CAT(name, _##idx)) \
		? _meta_requests_names[idx]

#define YARMI_GENERATE_METACODE_HAS_REQUEST_AUX(unused, idx, seq) \
	YARMI_GENERATE_METACODE_HAS_REQUEST_IMPL( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(idx, seq)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE_HAS_HANDLER_IMPL(idx, name) \
	BOOST_PP_IF(idx,:,) call_id == static_cast<id_type>(_meta_handlers_ids::BOOST_PP_CAT(name, _##idx)) \
		? _meta_handlers_names[idx]

#define YARMI_GENERATE_METACODE_HAS_HANDLER_AUX(unused, idx, seq) \
	YARMI_GENERATE_METACODE_HAS_HANDLER_IMPL( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(idx, seq)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE(ns, cn, oppons, oppocn, seq, opposeq) \
	private: \
		static constexpr const char *_meta_requests_names[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_METACODE_REQUESTS_NAMES_AUX \
				,(oppons, oppocn, seq) \
			) \
			0 \
		}; \
		enum class _meta_requests_ids: id_type { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_METACODE_REQUESTS_ENUM_AUX \
				,seq \
			) \
		}; \
		\
		static constexpr const char *_meta_handlers_names[] = { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(opposeq) \
				,YARMI_GENERATE_METACODE_HANDLERS_NAMES_AUX \
				,(ns, cn, opposeq) \
			) \
			0 \
		}; \
		enum class _meta_handlers_ids: id_type { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(opposeq) \
				,YARMI_GENERATE_METACODE_HANDLERS_ENUM_AUX \
				,opposeq \
			) \
		}; \
		\
	public: \
		static constexpr const char** meta_requests() { return _meta_requests_names; } \
		static constexpr std::size_t  meta_requests_count() { return (sizeof(_meta_requests_names)/sizeof(_meta_requests_names[0]))-1; } \
		static constexpr const char*  meta_request_name(const id_type call_id) { \
			return ( \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(seq) \
					,YARMI_GENERATE_METACODE_HAS_REQUEST_AUX \
					,seq \
				) \
				: 0 \
			); \
		} \
		static constexpr bool meta_has_request(const id_type call_id) { return meta_request_name(call_id) != 0; } \
		static constexpr bool meta_has_request(const char *str) { return meta_has_request(::yarmi::detail::fnv1a(str)); } \
		\
		static constexpr const char** meta_handlers() { return _meta_handlers_names; } \
		static constexpr std::size_t  meta_handlers_count() { return (sizeof(_meta_handlers_names)/sizeof(_meta_handlers_names[0]))-1; } \
		static constexpr const char*  meta_handler_name(const id_type call_id) { \
			return ( \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(opposeq) \
					,YARMI_GENERATE_METACODE_HAS_HANDLER_AUX \
					,opposeq \
				) \
				: 0 \
			); \
		} \
		static constexpr bool meta_has_handler(const id_type call_id) { return meta_handler_name(call_id) != 0; } \
		static constexpr bool meta_has_handler(const char *str) { return meta_has_handler(::yarmi::detail::fnv1a(str)); }

/***************************************************************************/

#endif // _yarmi__generate_metacode_hpp
