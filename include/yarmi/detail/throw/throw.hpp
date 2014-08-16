
// Copyright (c) 2013,2014, niXman (i dotty nixman doggy gmail dotty com)
// All rights reserved.
//
// This file is part of THROW(https://github.com/niXman/throw) project.
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

#ifndef _throw__throw_hpp
#define _throw__throw_hpp

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include <boost/format.hpp>

#include <stdexcept>
#include <sstream>

/***************************************************************************/
// based on the: http://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments

#define __THROW_YARMI_ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define __THROW_YARMI_HAS_COMMA(...) __THROW_YARMI_ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define __THROW_YARMI__TRIGGER_PARENTHESIS_(...) ,

#define __THROW_YARMI_ISEMPTY(...) \
	__THROW_YARMI__ISEMPTY( \
		/* test if there is just one argument, eventually an empty one */ \
		__THROW_YARMI_HAS_COMMA(__VA_ARGS__), \
		/* test if _TRIGGER_PARENTHESIS_ together with the argument adds a comma */ \
		__THROW_YARMI_HAS_COMMA(__THROW_YARMI__TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
		/* test if the argument together with a parenthesis adds a comma */ \
		__THROW_YARMI_HAS_COMMA(__VA_ARGS__ (/*empty*/)), \
		/* test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */ \
		__THROW_YARMI_HAS_COMMA(__THROW_YARMI__TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/)) \
	)

#define __THROW_YARMI_PASTE5(_0, _1, _2, _3, _4) \
	_0 ## _1 ## _2 ## _3 ## _4

#define __THROW_YARMI__ISEMPTY(_0, _1, _2, _3) \
	__THROW_YARMI_HAS_COMMA(__THROW_YARMI_PASTE5(__THROW_YARMI__IS_EMPTY_CASE_, _0, _1, _2, _3))

#define __THROW_YARMI__IS_EMPTY_CASE_0001 \
	,

#define __THROW_YARMI_TUPLE_IS_EMPTY(...) \
	__THROW_YARMI_ISEMPTY(__VA_ARGS__)

/***************************************************************************/

#define __THROW_YARMI_PASTE_ELEM(unused, idx, args) \
	% (BOOST_PP_TUPLE_ELEM(idx, args))

#define __THROW_YARMI_FORMAT_MESSAGE_WITHOUT_ARGS(...)

#define __THROW_YARMI_FORMAT_MESSAGE_WITH_ARGS(...) \
	BOOST_PP_REPEAT( \
		 BOOST_PP_TUPLE_SIZE((__VA_ARGS__)) \
		,__THROW_YARMI_PASTE_ELEM \
		,(__VA_ARGS__) \
	)

/***************************************************************************/

/* usage:
 * std::cout << YARMI_FORMAT_MESSAGE("message: %1%, %2%, %3%", "string1", 33, "string2") << std::endl;
 * std::string str = YARMI_FORMAT_MESSAGE_AS_STRING("message: %1%, %2%, %3%", "string1", 33, "string2");
 */

#define YARMI_FORMAT_MESSAGE(fmt, ...) \
	boost::format( \
		static_cast<const std::ostringstream&>( \
			std::ostringstream() \
				<< __FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ")[" \
				<< __PRETTY_FUNCTION__ << "]: " \
				<< fmt \
		).str() \
	) \
	BOOST_PP_IF( \
		 __THROW_YARMI_TUPLE_IS_EMPTY(__VA_ARGS__) \
		,__THROW_YARMI_FORMAT_MESSAGE_WITHOUT_ARGS \
		,__THROW_YARMI_FORMAT_MESSAGE_WITH_ARGS \
	)(__VA_ARGS__)

#define YARMI_FORMAT_MESSAGE_AS_STRING(fmt, ...) \
	(YARMI_FORMAT_MESSAGE(fmt, __VA_ARGS__)).str()

/***************************************************************************/

/* usage:
 * YARMI_THROW("message: %1%, %2%, %3%", "string1", 33, "string2");
 */

#define YARMI_THROW(fmt, ...) \
	throw std::runtime_error(YARMI_FORMAT_MESSAGE_AS_STRING(fmt, __VA_ARGS__))

/***************************************************************************/

/* usage:
 * YARMI_TYPED_THROW(std::logic_error, "message: %1%, %2%, %3%", "string1", 33, "string2");
 */

#define YARMI_TYPED_THROW(extype, fmt, ...) \
	throw extype(YARMI_FORMAT_MESSAGE_AS_STRING(fmt, __VA_ARGS__))

/***************************************************************************/

/* usage:
 * YARMI_TRY(exception_is_thrown) {
 *    ...code...
 * } YARMI_CATCH(exception_is_thrown)
 * if ( exception_is_thrown ) {
 *    ...processing...
 * }
 *
 * usage:
 * YARMI_TRY(exception_is_thrown) {
 *    ...code...
 * } YARMI_CATCH_LOG(exception_is_thrown, std::cerr)
 * if ( exception_is_thrown ) {
 *    ...processing...
 * }
 */

#define YARMI_TRY(flagname) \
	bool flagname = false; \
	((void)flagname); \
	try

#define YARMI_CATCH(flagname, ...) \
	catch (const std::exception &) { \
		flagname = true; \
		{ __VA_ARGS__; } \
	} catch (...) { \
		flagname = true; \
		{ __VA_ARGS__; } \
	}

#define YARMI_CATCH_LOG(flagname, logstream, ...) \
	catch (const std::exception &ex) { \
		flagname = true; \
		logstream << YARMI_FORMAT_MESSAGE("[exception]: %1%", ex.what()) << std::endl; \
		{ __VA_ARGS__; } \
	} catch (...) { \
		flagname = true; \
		logstream << YARMI_FORMAT_MESSAGE("[exception]: unknown exception") << std::endl; \
		{ __VA_ARGS__; } \
	}

/***************************************************************************/

#define YARMI_TEST_TYPED_THROW(expr, type) \
	do { \
		if ( !(expr) ) { \
			throw type( \
				YARMI_FORMAT_MESSAGE_AS_STRING("[test failed]: expression: \"" #expr "\"") \
			); \
		} \
	} while(0)

#define YARMI_TEST_THROW(expr) \
	YARMI_TEST_TYPED_THROW(expr, std::runtime_error)

/***************************************************************************/

#endif // _throw__throw_hpp
