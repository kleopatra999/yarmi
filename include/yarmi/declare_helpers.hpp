
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

#ifndef _yarmi__declare_helpers_hpp
#define _yarmi__declare_helpers_hpp

/***************************************************************************/

#define YARMI_GENERATE_HELPERS_GEN_PROC_NAME_proc(ns, cn, name, tuple) \
	case fnv1a_32(YARMI_NS_TO_STRING(ns, cn::name tuple)): return YARMI_NS_TO_STRING(ns, cn::name tuple);
#define YARMI_GENERATE_HELPERS_GEN_PROC_NAME_code(ns, cn, ...)

#define YARMI_GENERATE_HELPERS_GET_PROC_NAME_proc(...) \
	YARMI_GENERATE_HELPERS_GEN_PROC_NAME_proc
#define YARMI_GENERATE_HELPERS_GET_PROC_NAME_code(...) \
	YARMI_GENERATE_HELPERS_GEN_PROC_NAME_code

#define YARMI_GENERATE_HELPERS_GET_PROC_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_HELPERS_GET_PROC_NAME_, str)

#define YARMI_GENERATE_HELPERS_GET_ARGS_NAME_proc(...) \
	__VA_ARGS__
#define YARMI_GENERATE_HELPERS_GET_ARGS_NAME_code(...) \
	__VA_ARGS__

#define YARMI_GENERATE_HELPERS_GET_ARGS_NAME(str) \
	BOOST_PP_CAT(YARMI_GENERATE_HELPERS_GET_ARGS_NAME_, str)

#define YARMI_GENERATE_HELPERS_ONE_ITEM_IMPL(ns, cn, name, tuple) \
	name(ns, cn, tuple)

#define YARMI_GENERATE_HELPERS_ONE_ITEM(unused, idx, tuple) \
	YARMI_GENERATE_HELPERS_ONE_ITEM_IMPL( \
		 BOOST_PP_TUPLE_ELEM(0, tuple) \
		,BOOST_PP_TUPLE_ELEM(1, tuple) \
		,YARMI_GENERATE_HELPERS_GET_PROC_NAME(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, tuple))) \
		,YARMI_GENERATE_HELPERS_GET_ARGS_NAME(BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, tuple))) \
	)

#define YARMI_GENERATE_HELPERS(ns, cn, seq) \
	private: \
		static const char* yarmi_handler_name_impl(const std::uint32_t call_id) { \
			switch ( call_id ) { \
				BOOST_PP_REPEAT( \
					 BOOST_PP_SEQ_SIZE(seq) \
					,YARMI_GENERATE_HELPERS_ONE_ITEM \
					,(ns, cn, seq) \
				) \
				default: return 0; \
			} \
		} \
	public: \
		static const char* yarmi_handler_name(const std::uint32_t call_id) { \
			const char *str = yarmi_handler_name_impl(call_id); \
			return (str ? str : "unknown"); \
		} \
		static bool yarmi_has_handler(const std::uint32_t call_id) { \
			return yarmi_handler_name_impl(call_id) != 0; \
		}

/***************************************************************************/

#endif // _yarmi__declare_helpers_hpp
