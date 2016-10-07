
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

#ifndef _yarmi__serializers__binary_serializer_base_hpp
#define _yarmi__serializers__binary_serializer_base_hpp

#include <yarmi/yarmi_fwd.hpp>
#include <yarmi/detail/throw/throw.hpp>

#include <boost/noncopyable.hpp>

namespace yarmi {

/***************************************************************************/

struct binary_serializer_base: private boost::noncopyable {
	static constexpr std::size_t header_size() {
		static_assert(sizeof(std::uint32_t) == sizeof(yarmi::call_id_type), "bad types");

		return  sizeof(std::uint32_t) // body size
				+ sizeof(call_id_type)  // call id
		;
	}

	template<typename OS>
	static void reserve_space_for_header(OS &os) {
		static const char header_buffer[header_size()] = "\0";
		YARMI_TEST_THROW(os.write(header_buffer, header_size()) == header_size());
	}

	static void pack_header(const call_id_type call_id, const char *ptr, const std::size_t size) {
		std::uint32_t *header = (std::uint32_t*)ptr;
		// set body size
		header[0] = host_to_network((std::uint32_t)(size-header_size()));
		// set call id
		header[1] = host_to_network(call_id);
	}
	static std::pair<std::uint32_t, call_id_type>
	unpack_header(const char *ptr, const std::size_t size) {
		YARMI_TEST_THROW(size >= header_size());
		const std::uint32_t *header = (std::uint32_t*)ptr;
		std::pair<std::uint32_t, call_id_type> res = {
			// extract body size
			network_to_host(header[0]),
			// extract call id
			network_to_host(header[1])
		};

		return res;
	}
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__serializers__binary_serializer_base_hpp
