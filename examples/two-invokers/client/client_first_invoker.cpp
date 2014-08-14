
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

#include "client_first_invoker.hpp"
#include "client.hpp"

#include <iostream>

struct client_first_invoker_impl::impl {
	impl(client &c)
		:c(c)
	{}

	client &c;
}; // struct struct client_first_invoker_impl::impl

client_first_invoker_impl::client_first_invoker_impl(client &c)
	:two_invokers::client_first_invoker<client_first_invoker_impl, client>(*this, c)
	,pimpl(new impl(c))
{}

client_first_invoker_impl::~client_first_invoker_impl()
{ delete pimpl; }

void client_first_invoker_impl::on_pong(const std::string &str) {
	//std::cout << "client_first_invoker_impl::on_pong(" << str << ")" << std::endl;
	static std::size_t msgidx = 0;
	ping("message " + std::to_string(++msgidx));

	static std::size_t idx = 0;
	if ( ++idx == 1024 ) {
		idx = 0;
		std::cout << "received: \"" << str << "\"" << std::endl;
	}
}
