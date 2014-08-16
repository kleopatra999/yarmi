
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

#ifndef _yarmi__detail__pp__generate_invoker_hpp
#define _yarmi__detail__pp__generate_invoker_hpp

/***************************************************************************/

#define YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_EMPTY_ARGS(idx, name) \
	YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name)(impl);

#define YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_MEMBERS(unused, idx, tuple) \
	BOOST_PP_TUPLE_ELEM(idx, tuple) arg##idx;

#define YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_NONEMPTY_ARGS(idx, name, tuple) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_TUPLE_SIZE(tuple) \
		,YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_MEMBERS \
		,tuple \
	) \
	ser.deserialize(BOOST_PP_ENUM_PARAMS(BOOST_PP_TUPLE_SIZE(tuple), arg)); \
	YARMI_GENERATE_INVOKERS_SFINAE_GENERATE_SFINAE_NAME(idx, name)( \
		 impl\
		,BOOST_PP_ENUM_PARAMS(BOOST_PP_TUPLE_SIZE(tuple), arg) \
	);

#define YARMI_GENERATE_INVOKERS_ONE_ITEM(idx, name, tuple) \
	case static_cast<::yarmi::call_id_type>(_meta_handlers_ids::BOOST_PP_CAT(name, _##idx)): { \
		YARMI_LAZY_IF( \
			 YARMI_TUPLE_IS_EMPTY(tuple) \
			,(idx, name) \
			,(idx, name, tuple) \
			,YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_EMPTY_ARGS \
			,YARMI_GENERATE_INVOKERS_GENERATE_INVOKING_FOR_NONEMPTY_ARGS \
		) \
		return true; \
	}

#define YARMI_GENERATE_INVOKERS_IMPL(unused, idx, seq) \
	YARMI_GENERATE_INVOKERS_ONE_ITEM( \
		 idx \
		,BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(idx, seq)) \
		,BOOST_PP_TUPLE_ELEM(2, BOOST_PP_SEQ_ELEM(idx, seq)) \
	)

#define YARMI_GENERATE_INVOKERS(seq) \
	bool invoke(const ::yarmi::call_id_type call_id, serializer &ser) { \
		switch ( call_id ) { \
			BOOST_PP_REPEAT( \
				 BOOST_PP_SEQ_SIZE(seq) \
				,YARMI_GENERATE_INVOKERS_IMPL \
				,seq \
			) \
			default: return false; \
		} \
	} \
	\
	bool invoke(const ::yarmi::buffer_pair &buffer, ::yarmi::call_id_type *cid = 0) { \
		serializer ser(buffer); \
		::yarmi::call_id_type call_id = ser.get_call_id(); \
		\
		if ( cid ) \
			*cid = call_id; \
		\
		return invoke(call_id, ser); \
	} \

/***************************************************************************/

#endif // _yarmi__detail__pp__generate_invoker_hpp
