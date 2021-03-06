
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

#ifndef _yarmi__detail__pp__generate_ns_hpp
#define _yarmi__detail__pp__generate_ns_hpp

/***************************************************************************/

#define YARMI_GENERATE_OPEN_NS_ITEM(unused1, unused2, elem) \
	namespace elem {

#define YARMI_GENERATE_OPEN_NS(seq) \
	BOOST_PP_SEQ_FOR_EACH( \
		 YARMI_GENERATE_OPEN_NS_ITEM \
		,~ \
		,seq \
	)

#define YARMI_GENERATE_CLOSE_NS_ITEM(unused1, unused2, unused3) \
	}

#define YARMI_GENERATE_CLOSE_NS(seq) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_GENERATE_CLOSE_NS_ITEM \
		,~ \
	)

/***************************************************************************/

#define YARMI_GENERATE_USERCODE_NS(client_or_server, client_invoker_name, server_invoker_name) \
	BOOST_PP_IF( \
		 client_or_server \
		,BOOST_PP_CAT(server_invoker_name, BOOST_PP_CAT(__, BOOST_PP_CAT(client_invoker_name, __usercode))) \
		,BOOST_PP_CAT(client_invoker_name, BOOST_PP_CAT(__, BOOST_PP_CAT(server_invoker_name, __usercode))) \
	)

/***************************************************************************/

#define YARMI_GENERATE_NS_TO_STRING_ITEM(unused, idx, seq) \
	BOOST_PP_IF(idx,::BOOST_PP_SEQ_ELEM(idx, seq),BOOST_PP_SEQ_ELEM(idx, seq))

#define YARMI_GENERATE_NS_TO_STRING(seq, sym) \
	BOOST_PP_STRINGIZE(BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(seq),YARMI_GENERATE_NS_TO_STRING_ITEM,seq)::sym)

/***************************************************************************/

#define YARMI_GENERATE_USING_NS_ITEM(unused, idx, seq) \
	BOOST_PP_IF(idx,::BOOST_PP_SEQ_ELEM(idx, seq),BOOST_PP_SEQ_ELEM(idx, seq))

#define YARMI_GENERATE_USING_NS(seq) \
	BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(seq),YARMI_GENERATE_USING_NS_ITEM,seq)

/***************************************************************************/

#endif // _yarmi__detail__pp__generate_ns_hpp
