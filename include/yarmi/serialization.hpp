
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

#ifndef _yarmi__serialization_hpp
#define _yarmi__serialization_hpp

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

#include <yas/mem_streams.hpp>
#include <yas/serializers/std_types_serializers.hpp>

/***************************************************************************/

#endif // _yarmi__serialization_hpp
