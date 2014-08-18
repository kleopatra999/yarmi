
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

#ifndef _yarmi__invoke_hpp
#define _yarmi__invoke_hpp

#include <boost/mpl/size.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/unique.hpp>

#include <yarmi/yarmi_fwd.hpp>

namespace yarmi {
namespace detail {
namespace mpl {

template<typename V, typename... Vs>
struct cat;

template<typename T>
struct cat<T> {
	using type = typename T::_meta_handlers_ids_vec;
};

template<typename V, typename... Vs>
struct cat {
	using type = typename boost::mpl::copy<
		 typename V::_meta_handlers_ids_vec
		,boost::mpl::back_inserter<typename cat<Vs...>::type>
	>::type;
};

template<typename V>
struct unique {
	using type = typename boost::mpl::unique<
		 typename boost::mpl::sort<V>::type
		,boost::is_same<boost::mpl::_1, boost::mpl::_2>
	>::type;
};

template<typename V>
using size = boost::mpl::size<V>;

} // ns mpl
} // ns detail

/***************************************************************************/

bool invoke(const call_id_type, const buffer_pair &) { return false; }
template<typename Invoker, typename... Invokers>
bool invoke(const call_id_type call_id, const buffer_pair &buffer, Invoker &head, Invokers&... tail) {
	using ids    = typename detail::mpl::cat<Invoker, Invokers...>::type;
	using unique = typename detail::mpl::unique<ids>::type;
	static_assert(
		 detail::mpl::size<ids>::value == detail::mpl::size<unique>::value
		,"proc IDs collision is detected"
	);

	return head.invoke(call_id, buffer) || invoke(call_id, buffer, tail...);
}

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__invoke_hpp
