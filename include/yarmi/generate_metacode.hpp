
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

#ifndef _yarmi__declare_metacode_hpp
#define _yarmi__declare_metacode_hpp

/***************************************************************************/

#define YARMI_GENERATE_METACODE_GET_ID_VAR_NAME(idx, tuple) \
	BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(1, tuple), _##idx)

#define YARMI_GENERATE_METACODE_IMPL(idx, ns, cn, tuple) \
	YARMI_GENERATE_METACODE_GET_ID_VAR_NAME(idx, tuple) = \
		::yarmi::detail::fnv1a_32( \
			YARMI_NS_TO_STRING( \
				 ns \
				,cn::BOOST_PP_TUPLE_ELEM(1, tuple) BOOST_PP_TUPLE_ELEM(2, tuple) \
			) \
		),

#define YARMI_GENERATE_METACODE_AUX(unused, idx, tuple) \
	YARMI_GENERATE_METACODE_IMPL( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(0, tuple) \
		,BOOST_PP_TUPLE_ELEM(1, tuple) \
		,BOOST_PP_SEQ_ELEM(idx, BOOST_PP_TUPLE_ELEM(2, tuple)) \
	)

/***************************************************************************/

#define YARMI_GENERATE_METACODE(ns, cn, opposeq, seq) \
	private: \
		enum class _yarmi_handlers: id_type { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_METACODE_AUX \
				,(ns, cn, seq) \
			) \
		}; \
		enum class _yarmi_requests: id_type { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(opposeq) \
				,YARMI_GENERATE_METACODE_AUX \
				,(ns, cn, opposeq) \
			) \
		}; \
	public:


#endif // _yarmi__declare_metacode_hpp
