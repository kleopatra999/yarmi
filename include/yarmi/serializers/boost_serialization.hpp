
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

#ifndef _yarmi__serializers__boost_serialization_hpp
#define _yarmi__serializers__boost_serialization_hpp

#include <yarmi/yarmi_fwd.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/noncopyable.hpp>

#include <sstream>
#include <tuple>

/***************************************************************************/

namespace yarmi {
namespace detail {

template<uint N>
struct Serialize {
	template<class Archive, typename... Args>
	static void serialize(Archive & ar, std::tuple<Args...> & t, const unsigned int version) {
	  ar & std::get<N-1>(t);
	  Serialize<N-1>::serialize(ar, t, version);
	}
};

template<>
struct Serialize<0> {
	template<class Archive, typename... Args>
	static void serialize(Archive &, std::tuple<Args...> &, const unsigned int)
	{}
};

} // ns detail
} // ns yarmi

namespace boost {
namespace serialization {

template<class Archive, typename... Args>
void serialize(Archive &ar, std::tuple<Args...> &t, const unsigned int version) {
	::yarmi::detail::Serialize<sizeof...(Args)>::serialize(ar, t, version);
}

} // ns serialization
} // ns boost

/***************************************************************************/

namespace yarmi {

struct boost_serializer: private boost::noncopyable {
	boost_serializer(const buffer_pair &buffer)
		:is(std::string(buffer.first.get(), buffer.second))
		,ia(is, boost::archive::no_header)
	{}

	template<typename... Args>
	static buffer_pair serialize(const call_id_type call_id, const Args&... args) {
		static const std::uint32_t body_size = 0;

		std::ostringstream os;
		os.write((char*)&body_size, sizeof(body_size));

		{
			boost::archive::binary_oarchive oa(os, boost::archive::no_header);
			oa & call_id;
			if ( sizeof...(Args) ) {
				const auto tuple = std::make_tuple(std::cref(args)...);
				oa & tuple;
			}
		}

		const std::string &str = os.str();
		std::uint32_t *size = (std::uint32_t*)str.c_str();
		*size = (std::uint32_t)(str.size()-sizeof(body_size));

		const buffer_pair buffer = allocate_buffer(str.size());
		std::memcpy(buffer.first.get(), str.c_str(), str.size());

		return buffer;
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
	std::istringstream is;
	boost::archive::binary_iarchive ia;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__serializers__boost_serialization_hpp
