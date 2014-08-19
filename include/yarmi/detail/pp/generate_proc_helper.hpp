
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

#ifndef _yarmi__detail__pp__generate_proc_helper_hpp
#define _yarmi__detail__pp__generate_proc_helper_hpp

/***************************************************************************/

#ifndef YARMI_DUMP_PROC_ARGS
#	define YARMI_DECLARE_PROC_WITHOUT_DUMP_FOR_EMPTY_ARGS(pname) \
		pname()

#	define YARMI_DECLARE_PROC_WITHOUT_DUMP_FOR_NONEMPTY_ARGS_PLACE_ONE_ARG(unused, idx, tuple) \
		const BOOST_PP_TUPLE_ELEM(idx, tuple)& arg##idx \
			YARMI_COMMA_IF_NOT_LAST_ITERATION(BOOST_PP_TUPLE_SIZE(tuple), idx)

#	define YARMI_DECLARE_PROC_WITHOUT_DUMP_FOR_NONEMPTY_ARGS(pname, tuple_) \
		pname( \
			BOOST_PP_REPEAT( \
				 BOOST_PP_TUPLE_SIZE(tuple_) \
				,YARMI_DECLARE_PROC_WITHOUT_DUMP_FOR_NONEMPTY_ARGS_PLACE_ONE_ARG \
				,tuple_ \
			) \
		)

#	define YARMI_DECLARE_PROC(tuple, ...) \
		YARMI_LAZY_IF( \
			 YARMI_TUPLE_IS_EMPTY((__VA_ARGS__)) \
			,(BOOST_PP_TUPLE_ELEM(1, tuple)) \
			,(BOOST_PP_TUPLE_ELEM(1, tuple), (__VA_ARGS__)) \
			,YARMI_DECLARE_PROC_WITHOUT_DUMP_FOR_EMPTY_ARGS \
			,YARMI_DECLARE_PROC_WITHOUT_DUMP_FOR_NONEMPTY_ARGS \
		)
#else // !YARMI_DUMP_PROC_ARGS
#	define YARMI_DECLARE_PROC_WITH_DUMP_FOR_EMPTY_ARGS(pname) \
		pname(const std::tuple<> &args)

#	define YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS_PLACE_ONE_ARG(unused, idx, tuple) \
		const BOOST_PP_TUPLE_ELEM(idx, tuple)& arg##idx \
		,

#	define YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS_EXPAND_TUPLE_ONE_ITEM(unused, idx, tuple) \
		const BOOST_PP_TUPLE_ELEM(idx, tuple) & \
			YARMI_COMMA_IF_NOT_LAST_ITERATION(BOOST_PP_TUPLE_SIZE(tuple), idx)

#	define YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS_EXPAND_TUPLE(tuple) \
		BOOST_PP_REPEAT( \
			 BOOST_PP_TUPLE_SIZE(tuple) \
			,YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS_EXPAND_TUPLE_ONE_ITEM \
			,tuple \
		)

#	define YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS(pname, tuple_) \
		pname( \
			BOOST_PP_REPEAT( \
				 BOOST_PP_TUPLE_SIZE(tuple_) \
				,YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS_PLACE_ONE_ARG \
				,tuple_ \
			) \
			const std::tuple< \
				YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS_EXPAND_TUPLE(tuple_) \
			> &args \
		)

#	define YARMI_DECLARE_PROC(pname, ...) \
		YARMI_LAZY_IF( \
			 YARMI_TUPLE_IS_EMPTY((__VA_ARGS__)) \
			,(pname) \
			,(pname, (__VA_ARGS__)) \
			,YARMI_DECLARE_PROC_WITH_DUMP_FOR_EMPTY_ARGS \
			,YARMI_DECLARE_PROC_WITH_DUMP_FOR_NONEMPTY_ARGS \
		)

#endif // YARMI_DUMP_PROC_ARGS

/***************************************************************************/

#endif // _yarmi__detail__pp__generate_proc_helper_hpp
