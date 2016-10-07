
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

#include "command_processor.hpp"
#include "client_session.hpp"

#include <unordered_map>
#include <functional>

#include <iostream>

/***************************************************************************/

struct command_processor::pimpl {
	pimpl(client_session &client)
		:client(client)
	{}

	client_session &client;

	std::unordered_map<std::string, std::function<void(const std::string&)>> cmds_map;
}; // struct pimpl

/***************************************************************************/

command_processor::command_processor(client_session &client)
	:impl(new pimpl(client))
{
	auto registration = [this](const std::string &cmd) {
		std::string username;
		while ( 1 ) {
			std::cout << "user name: ";
			std::cin >> username;
			if ( username.find(' ') == std::string::npos ) {
				break;
			} else {
				std::cout << "username can not contain spaces." << std::endl;
			}
		}

		impl->client.registration(username);
	};

	impl->cmds_map.emplace(":registration", registration);
}

command_processor::~command_processor()
{ delete impl; }

/***************************************************************************/

void command_processor::process(const std::string &text) {

}

/***************************************************************************/
