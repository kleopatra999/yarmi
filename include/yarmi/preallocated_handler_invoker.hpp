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

#ifndef _yarmi__preallocated_handler_invoker_hpp
#define _yarmi__preallocated_handler_invoker_hpp

#include <boost/asio.hpp>
#include <memory>

namespace yarmi {

/***************************************************************************/

template<typename Allocator, typename H>
struct preallocated_handler_invoker {
	typedef preallocated_handler_invoker<Allocator, H> this_type;

	preallocated_handler_invoker(Allocator &allocator , H h)
		:allocator(std::addressof(allocator))
		,handler(h)
	{}

	friend void* asio_handler_allocate(std::size_t size, this_type *ctx) {
		return ctx->allocator->allocate(size);
	}

	friend void asio_handler_deallocate(void *ptr, std::size_t, this_type *ctx) {
		ctx->allocator->deallocate(ptr);
	}

	template <typename F>
	friend void asio_handler_invoke(F& function, this_type *context) {
		using boost::asio::asio_handler_invoke;
		asio_handler_invoke(function, std::addressof(context->handler));
	}

	template<typename... Args>
	void operator()(Args&&... args) {
		handler(std::forward<Args>(args)...);
	}

private:
	Allocator* allocator;
	H handler;
};

/***************************************************************************/

} // namespace yarmi

#endif // _yarmi__preallocated_handler_invoker_hpp
