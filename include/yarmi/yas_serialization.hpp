
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

#ifndef _yarmi__yas_serialization_hpp
#define _yarmi__yas_serialization_hpp

#include <yas/mem_streams.hpp>
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/serializers/std_types_serializers.hpp>

#include <yarmi/yarmi_fwd.hpp>

namespace yarmi {

/***************************************************************************/

struct yas_serializer {
	yas_serializer(const buffer_pair &buffer)
		:is(buffer.first.get(), buffer.second)
		,ia(is)
	{}

	template<typename... Args>
	static buffer_pair serialize(const yarmi::call_id_type call_id, const Args&... args) {
		static const std::uint32_t body_size = 0;

		yas::mem_ostream os;
		os.write(&body_size, sizeof(body_size));

		yas::binary_oarchive<yas::mem_ostream> oa(os);
		oa & call_id;
		if ( sizeof...(Args) ) {
			oa & std::make_tuple(std::cref(args)...);
		}

		const yas::intrusive_buffer &intrusive_buffer = os.get_intrusive_buffer();
		std::uint32_t *size = (std::uint32_t*)((char*)intrusive_buffer.data);
		*size = intrusive_buffer.size-sizeof(body_size);

		const yas::shared_buffer &shared_buffer = os.get_shared_buffer();
		return {shared_buffer.data, shared_buffer.size};
	}

	call_id_type get_call_id() {
		call_id_type call_id = 0;
		ia & call_id;
		return call_id;
	}
	template<typename... Args>
	void deserialize(Args&... args) {
		if ( sizeof...(Args) ) {
			auto tuple = std::make_tuple(std::ref(args)...);
			ia & tuple;
		}
	}

private:
	yas::mem_istream is;
	yas::binary_iarchive<yas::mem_istream> ia;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__yas_serialization_hpp
