
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

#include "client_session.hpp"

#include <yarmi/invoke.hpp>

#include <iostream>

/***************************************************************************/

client_session::client_session(boost::asio::io_service &ios)
	:yarmi::client_base(ios)
	,yarmi::client_invoker<client_session>(*this, *this)
{}

/***************************************************************************/

void client_session::on_received(const yarmi::buffer_pair &buffer) {
	yarmi::call_id_type call_id = 0;
	YARMI_TRY(invoke_flag) {
		const bool ok = yarmi::invoke(buffer, &call_id, *this);
		if ( ! ok ) {
			std::cerr << "client::invoke(): no proc for call_id=" << call_id << std::endl;
		}
	} YARMI_CATCH_LOG(invoke_flag, std::cerr)
}

/***************************************************************************/

void client_session::on_registration(const std::string &msg, const std::string &regkey) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you activation key is: " << regkey << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_activation(const std::string &msg) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you successfully registered" << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_login(const std::string &msg) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you successfully logged in" << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_logout(const std::string &msg) {
	if ( msg.empty() ) {
		std::cout << "> OK -> you successfully logged out" << std::endl;
	} else {
		std::cout << "> ERROR -> " << msg << std::endl;
	}
}

/***************************************************************************/

void client_session::on_users_online(const std::vector<std::string> &users) {
	if ( users.empty() ) {
		std::cout << "no users online" << std::endl;
		return;
	}

	std::cout << "users online:" << std::endl;
	for ( const auto &it: users ) {
		std::cout << "   " << it << std::endl;
	}
}

/***************************************************************************/
