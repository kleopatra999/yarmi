
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

#ifndef _yarmi__server__handler_allocator_hpp
#define _yarmi__server__handler_allocator_hpp

#include <memory>
#include <type_traits>
#include <boost/noncopyable.hpp>

namespace yarmi {

/***************************************************************************/

template<std::size_t alloc_size>
struct handler_allocator: private boost::noncopyable {
	handler_allocator(const handler_allocator&) = delete;
	handler_allocator& operator=(const handler_allocator&) = delete;

	handler_allocator()
		:in_use(false)
	{}

	void* allocate(std::size_t size) {
		if ( !in_use && size <= alloc_size ) {
			in_use = true;
			return std::addressof(storage);
		}

		return ::operator new(size);
	}

	void deallocate(void *pointer) {
		if ( std::addressof(storage) == pointer ) {
			in_use = false;
		} else {
			::operator delete(pointer);
		}
	}

private:
	typename std::aligned_storage<alloc_size>::type storage;
	bool in_use;
};

/***************************************************************************/

} // namespace yarmi

#endif // _yarmi__server__handler_allocator_hpp
