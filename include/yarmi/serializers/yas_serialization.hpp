
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

#ifndef _yarmi__serializers__yas_serialization_hpp
#define _yarmi__serializers__yas_serialization_hpp

#include <yarmi/serializers/binary_serializer_base.hpp>

#include <yas/mem_streams.hpp>
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/serializers/std_types_serializers.hpp>

namespace yarmi {

/***************************************************************************/

struct yas_serializer: binary_serializer_base {
	template<typename... Args>
	static buffer_pair serialize(const call_id_type call_id, const Args&... args) {
		yas::mem_ostream os;
		reserve_space_for_header(os);

		if ( sizeof...(Args) ) {
			yas::binary_oarchive<yas::mem_ostream> oa(os);
			oa & std::make_tuple(std::cref(args)...);
		}

		const yas::intrusive_buffer &intrusive_buffer = os.get_intrusive_buffer();
		pack_header(call_id, intrusive_buffer.data, intrusive_buffer.size);

		const yas::shared_buffer &shared_buffer = os.get_shared_buffer();
		return {shared_buffer.data, shared_buffer.size};
	}

	template<typename... Args>
	static void deserialize(const buffer_pair &buffer, Args&... args) {
		if ( sizeof...(Args) ) {
			yas::mem_istream is(buffer.first.get(), buffer.second);
			yas::binary_iarchive<yas::mem_istream> ia(is);
			auto tuple = std::make_tuple(std::ref(args)...);
			ia & tuple;
		}
	}
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__serializers__yas_serialization_hpp
