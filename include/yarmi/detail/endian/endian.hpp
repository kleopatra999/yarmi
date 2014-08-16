
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

#ifndef _yarmi__detail__endian__endian_hpp
#define _yarmi__detail__endian__endian_hpp

#include <cstdint>

#if defined (__GLIBC__)
#	include <endian.h>
#	if (__BYTE_ORDER == __LITTLE_ENDIAN)
#		define YARMI_LITTLE_ENDIAN (1)
#		define YARMI_BIG_ENDIAN (0)
#	elif (__BYTE_ORDER == __BIG_ENDIAN)
#		define YARMI_LITTLE_ENDIAN (0)
#		define YARMI_BIG_ENDIAN (1)
#	elif (__BYTE_ORDER == __PDP_ENDIAN)
#		define YARMI_PDP_ENDIAN
#	else
#		error Unknown machine endianness detected.
#	endif
#	define YARMI_BYTE_ORDER __BYTE_ORDER
#elif defined(_BIG_ENDIAN)
#	define YARMI_LITTLE_ENDIAN (0)
#	define YARMI_BIG_ENDIAN (1)
#	define YARMI_BYTE_ORDER 4321
#elif defined(_LITTLE_ENDIAN)
#	define YARMI_LITTLE_ENDIAN (1)
#	define YARMI_BIG_ENDIAN (0)
#	define YARMI_BYTE_ORDER 1234
#elif defined(__sparc) || defined(__sparc__) \
	|| defined(_POWER) || defined(__powerpc__) \
	|| defined(__ppc__) || defined(__hpux) \
	|| defined(_MIPSEB) || defined(_POWER) \
	|| defined(__s390__)
#	define YARMI_LITTLE_ENDIAN (0)
#	define YARMI_BIG_ENDIAN (1)
#	define YARMI_BYTE_ORDER 4321
#elif defined(__i386__) || defined(__alpha__) \
	|| defined(__ia64) || defined(__ia64__) \
	|| defined(_M_IX86) || defined(_M_IA64) \
	|| defined(_M_ALPHA) || defined(__amd64) \
	|| defined(__amd64__) || defined(_M_AMD64) \
	|| defined(__x86_64) || defined(__x86_64__) \
	|| defined(_M_X64)
#	define YARMI_LITTLE_ENDIAN (1)
#	define YARMI_BIG_ENDIAN (0)
#	define YARMI_BYTE_ORDER 1234
#else
#	error bad CPU type.
#endif

#if defined(__clang__) || defined(__GNUC__)
#	if YARMI_LITTLE_ENDIAN
#		define YARMI_HOST_TO_NETWORK_16(v) __builtin_bswap16((v))
#		define YARMI_HOST_TO_NETWORK_32(v) __builtin_bswap32((v))
#		define YARMI_HOST_TO_NETWORK_64(v) __builtin_bswap64((v))
#		define YARMI_NETWORK_TO_HOST_16(v) YARMI_HOST_TO_NETWORK_16((v))
#		define YARMI_NETWORK_TO_HOST_32(v) YARMI_HOST_TO_NETWORK_32((v))
#		define YARMI_NETWORK_TO_HOST_64(v) YARMI_HOST_TO_NETWORK_64((v))
#	else // ! ifdef YARMI_LITTLE_ENDIAN
#		define YARMI_HOST_TO_NETWORK_16(v) (v)
#		define YARMI_HOST_TO_NETWORK_32(v) (v)
#		define YARMI_HOST_TO_NETWORK_64(v) (v)
#		define YARMI_NETWORK_TO_HOST_16(v) (v)
#		define YARMI_NETWORK_TO_HOST_32(v) (v)
#		define YARMI_NETWORK_TO_HOST_64(v) (v)
#	endif // ifdef YARMI_LITTLE_ENDIAN
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#	error compiler is not configured
#elif defined(_MSC_VER)
#	if YARMI_LITTLE_ENDIAN
#		define YARMI_HOST_TO_NETWORK_16(v) _byteswap_ushort((v))
#		define YARMI_HOST_TO_NETWORK_32(v) _byteswap_ulong((v))
#		define YARMI_HOST_TO_NETWORK_64(v) _byteswap_uint64((v))
#		define YARMI_NETWORK_TO_HOST_16(v) YARMI_HOST_TO_NETWORK_16((v))
#		define YARMI_NETWORK_TO_HOST_32(v) YARMI_HOST_TO_NETWORK_32((v))
#		define YARMI_NETWORK_TO_HOST_64(v) YARMI_HOST_TO_NETWORK_64((v))
#	else // ! ifdef YARMI_LITTLE_ENDIAN
#		define YARMI_HOST_TO_NETWORK_16(v) (v)
#		define YARMI_HOST_TO_NETWORK_32(v) (v)
#		define YARMI_HOST_TO_NETWORK_64(v) (v)
#		define YARMI_NETWORK_TO_HOST_16(v) (v)
#		define YARMI_NETWORK_TO_HOST_32(v) (v)
#		define YARMI_NETWORK_TO_HOST_64(v) (v)
#	endif // ifdef YARMI_LITTLE_ENDIAN
#else
#  error unknown compiler
#endif

/***************************************************************************/

namespace yarmi {
namespace detail {

template<std::size_t S>
struct endian;

template<>
struct endian<sizeof(std::uint16_t)> {
	static std::uint16_t host_to_network(const std::uint16_t v) { return YARMI_HOST_TO_NETWORK_16(v); }
	static std::uint16_t network_to_host(const std::uint16_t v) { return YARMI_NETWORK_TO_HOST_16(v); }
};
template<>
struct endian<sizeof(std::uint32_t)> {
	static std::uint32_t host_to_network(const std::uint32_t v) { return YARMI_HOST_TO_NETWORK_32(v); }
	static std::uint32_t network_to_host(const std::uint32_t v) { return YARMI_NETWORK_TO_HOST_32(v); }
};
template<>
struct endian<sizeof(std::uint64_t)> {
	static std::uint64_t host_to_network(const std::uint64_t v) { return YARMI_HOST_TO_NETWORK_64(v); }
	static std::uint64_t network_to_host(const std::uint64_t v) { return YARMI_NETWORK_TO_HOST_64(v); }
};

} // ns detail

/***************************************************************************/

template<typename T>
T host_to_network(const T v) { return detail::endian<sizeof(T)>::host_to_network(v); }
template<typename T>
T network_to_host(const T v) { return detail::endian<sizeof(T)>::network_to_host(v); }

} // ns yarmi

/***************************************************************************/

#endif // _yarmi__detail__endian__endian_hpp
