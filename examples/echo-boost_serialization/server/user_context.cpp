
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

#include "user_context.hpp"
#include "global_context.hpp"

#include <yarmi/invoke.hpp>

#include <iostream>

/***************************************************************************/

user_context::user_context(const yarmi::socket_ptr &socket, yarmi::server_base &sb, global_context<user_context> &gc)
	:yarmi::session(socket, sb)
	,yarmi::server_invoker<user_context>(*this, *this)
	,gc(gc)
{}

/***************************************************************************/

void user_context::on_connected() {
	std::cout << YARMI_FORMAT_MESSAGE(
		 "on_connected(%1%) called"
		,get_socket().remote_endpoint().address().to_string()
	) << std::endl;
}

void user_context::on_disconnected() {
	std::cout << YARMI_FORMAT_MESSAGE("on_disconnected() called") << std::endl;
}

void user_context::on_received(const yarmi::buffer_pair &buffer) {
	YARMI_TRY(invoke_flag) {
		yarmi::call_id_type call_id = 0;
		if ( !yarmi::invoke(buffer, &call_id, *this) ) {
			std::cerr << YARMI_FORMAT_MESSAGE(
				 "no handler for call_id \"%1%\""
				,call_id
			) << std::endl;
		}
	} YARMI_CATCH_LOG(invoke_flag, std::cerr)
}

/***************************************************************************/

void user_context::on_ping(const std::string &msg) {
	pong(msg);
}

/***************************************************************************/