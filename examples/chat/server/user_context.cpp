
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

#include "global_context.hpp"
#include "user_context.hpp"

/***************************************************************************/

user_context::user_context(yarmi::server_base &sb, global_context<user_context> &gc)
	:yarmi::session_base(sb)
	,yarmi::server_invoker<user_context>(*this, *this)
	,gc(gc)
{}

/***************************************************************************/

void user_context::on_connected() {

}

/***************************************************************************/

void user_context::on_disconnected() {

}

/***************************************************************************/

void user_context::on_received(const char *ptr, std::size_t size) {
	invoke(ptr, size);
}

/***************************************************************************/

void user_context::on_registration(const std::string &username) {
	if ( gc.user_registered(username) ) {
		registration("username "+username+" already registered", "");
	} else {

	}
}

/***************************************************************************/

void user_context::on_activation(const std::string &regkey, const std::string &useranme, const std::string &pass) {

}

/***************************************************************************/

void user_context::on_login(const std::string &useranme, const std::string &pass) {

}

/***************************************************************************/

void user_context::on_logout() {

}

/***************************************************************************/

void user_context::on_users_online() {

}

void user_context::on_users_online(const std::string &substr) {

}

/***************************************************************************/
