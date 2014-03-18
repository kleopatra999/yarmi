
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

#ifndef _yarmi__declare_callers_hpp
#define _yarmi__declare_callers_hpp

/***************************************************************************/

#define YARMI_DECLARE_REMOTE_CALL_PARAMS(unused, idx, tuple) \
	const BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_SIZE(tuple), idx, tuple) &arg##idx \
		YARMI_COMMA_IF_NOT_LAST_ITERATION(BOOST_PP_TUPLE_SIZE(tuple), idx)


#define YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_EMPTY(idx, name, opponame) \
	void name() { \
		YARMI_OSTREAM_TYPE os, os2; \
		YARMI_OARCHIVE_TYPE oa(os, yas::no_header), pa(os2); \
		oa & static_cast<id_type>(_meta_requests_ids::BOOST_PP_CAT(name, _##idx)); \
		pa & os.get_intrusive_buffer(); \
		io.send(os2.get_shared_buffer()); \
	}

#define YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY_SERIALIZE(unused, idx, data) \
	& arg##idx

#define YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY(idx, name, opponame, tuple) \
	void name( \
		BOOST_PP_REPEAT( \
			BOOST_PP_TUPLE_SIZE(tuple), \
			YARMI_DECLARE_REMOTE_CALL_PARAMS, \
			tuple \
		) \
	) { \
		YARMI_OSTREAM_TYPE os, os2; \
		YARMI_OARCHIVE_TYPE oa(os, yas::no_header), pa(os2); \
		oa & static_cast<id_type>(_meta_requests_ids::BOOST_PP_CAT(name, _##idx)) \
		\
		BOOST_PP_REPEAT( \
				 BOOST_PP_TUPLE_SIZE(tuple) \
				,YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY_SERIALIZE \
				,~ \
			) \
		; \
		pa & os.get_intrusive_buffer(); \
		io.send(os2.get_shared_buffer()); \
	}

#define YARMI_GENERATE_CALLERS_ONE_ITEM_IMPL(idx, name, opponame, tuple) \
	YARMI_LAZY_IF( \
		YARMI_TUPLE_IS_EMPTY(tuple), \
		(idx, name, opponame), \
		(idx, name, opponame, tuple), \
		YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_EMPTY, \
		YARMI_DECLARE_REMOTE_CALL_FOR_ONE_VERSION_NONEMPTY \
	)

#define YARMI_GENERATE_CALLERS_ONE_ITEM(unused, idx, seq) \
	YARMI_GENERATE_CALLERS_ONE_ITEM_IMPL( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(idx, seq)) \
		,BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(idx, seq)) \
		,BOOST_PP_TUPLE_ELEM(2, BOOST_PP_SEQ_ELEM(idx, seq)) \
	)

#define YARMI_GENERATE_CALLERS(ns, cn, seq) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_SEQ_SIZE(seq) \
		,YARMI_GENERATE_CALLERS_ONE_ITEM \
		,seq \
	)

/***************************************************************************/

#endif // _yarmi__declare_callers_hpp
