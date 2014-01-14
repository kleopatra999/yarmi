
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

#include <protocol.hpp>

#include <yarmi/client_base.hpp>

#include <iostream>

/***************************************************************************/

struct client: yarmi::client_base<client>, yarmi::client_invoker<client> {
	client(boost::asio::io_service &ios)
		:yarmi::client_base<client>(ios, *this)
		,yarmi::client_invoker<client>(*this, *this)
		,msg_index(0)
	{}

	void on_pong(const std::string &msg) {
		std::cout << "received: \"" << msg << "\"" << std::endl;
		ping("my message "+std::to_string(++msg_index));
	}

	std::size_t msg_index;
};

/***************************************************************************/

int main() {
	static const char *ip = "127.0.0.1";
	static const std::uint16_t port = 44550;

	boost::asio::io_service ios;
	client c(ios);
	//c.connect(ip, port);
	c.async_connect(
		 ip
		,port
		,[&c](const boost::system::error_code &ec) {
			if ( ec ) {
				std::cout << "async_connect handler: " << ec.message() << std::endl;
				return;
			}
			c.start();
			c.ping("message");
		}
	);

	ios.run();

	return 0;
}

/***************************************************************************/
