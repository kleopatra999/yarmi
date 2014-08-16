
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

#include "protocol.hpp"

#include <yarmi/client/client_base.hpp>
#include <yarmi/invoke.hpp>

#include <iostream>
#include <map>
#include <functional>

/***************************************************************************/

struct client: yarmi::client_base, yarmi::client_side<client> {
	client(boost::asio::io_service &ios)
		:yarmi::client_base(ios)
		,yarmi::client_side<client>(*this, *this)
		,cmds{
			 std::make_pair("pwd"   , [this](const std::string & ){pwd();})
			,std::make_pair("mkdir" , [this](const std::string &a){mkdir(a);})
			,std::make_pair("touch" , [this](const std::string &a){touch(a);})
			,std::make_pair("rm"    , [this](const std::string &a){rm(a);})
			,std::make_pair("ls"    , [this](const std::string &a){ls(a);})
			,std::make_pair("cd"    , [this](const std::string &a){cd(a);})
		}
	{}

	void on_received(const yarmi::buffer_pair &buffer) {
		yarmi::call_id_type call_id = 0;
		YARMI_TRY(invoke_flag) {
			const bool ok = yarmi::invoke(buffer, &call_id, *this);
			if ( ! ok ) {
				std::cerr << "client::invoke(): no proc for call_id=" << call_id << std::endl;
			}
		} YARMI_CATCH_LOG(invoke_flag, std::cerr)
	}

	void read_cmd() {
		std::string line, cmd, args;
		while ( true ) {
			std::cout << "$ " << std::flush;
			std::getline(std::cin, line);

			if ( line.empty() ) continue;

			const auto pos = line.find(' ');
			if ( pos+1 == line.length() ) {
				std::cout << "bad command \"" << cmd << "\"" << std::endl;
				continue;
			}

			if ( pos == std::string::npos ) {
				cmd = line;
				args.clear();
			} else {
				cmd = line.substr(0, pos);
				args= line.substr(pos+1);
			}

			if ( cmd == "exit" ) {
				std::cout << "buy-buy! ;)" << std::endl;
				get_io_service().stop();
				return;
			}

			auto it = cmds.find(cmd);
			if ( it == cmds.end() ) {
				std::cout << "bad command \"" << cmd << "\"" << std::endl;
				continue;
			}
			it->second(args);

			break;
		}
	}

	/***************************************************************************/

	void print_error(std::ostream &os, int ec, const std::string &em) {
		if ( ec ) {
			os << "[error]: errno=" << ec << "\", message=\"" << em << "\"" << std::endl;
		}
	}

	/***************************************************************************/

	void on_pwd(int ec, const std::string &em, const std::string &res) {
		//std::cout << "on_pwd(): ec=" << ec << ", em=\"" << em << "\", res=\"" << res << "\"" << std::endl;
		print_error(std::cout, ec, em);
		std::cout << res << std::endl;
		read_cmd();
	}

	void on_mkdir(int ec, const std::string &em, const std::string &/*res*/) {
		//std::cout << "on_mkdir(): ec=" << ec << ", em=\"" << em << "\", res=\"" << res << "\"" << std::endl;
		print_error(std::cout, ec, em);
		read_cmd();
	}

	void on_touch(int ec, const std::string &em, const std::string &/*res*/) {
		//std::cout << "on_touch(): ec=" << ec << ", em=\"" << em << "\", res=\"" << res << "\"" << std::endl;
		print_error(std::cout, ec, em);
		read_cmd();
	}

	void on_rm(int ec, const std::string &em, const std::string &/*res*/) {
		//std::cout << "on_rm(): ec=" << ec << ", em=\"" << em << "\", res=\"" << res << "\"" << std::endl;
		print_error(std::cout, ec, em);
		read_cmd();
	}

	void on_ls(int ec, const std::string &em, const std::string &res) {
		//std::cout << "on_ls(): ec=" << ec << ", em=\"" << em << "\", res=\"" << res << "\"" << std::endl;
		print_error(std::cout, ec, em);
		std::cout << res << std::endl;
		read_cmd();
	}

	void on_cd(int ec, const std::string &em, const std::string &/*res*/) {
		//std::cout << "on_cd(): ec=" << ec << ", em=\"" << em << "\", res=\"" << res << "\"" << std::endl;
		print_error(std::cout, ec, em);
		read_cmd();
	}

private:
	std::map<std::string, std::function<void(const std::string &)>> cmds;
};

/***************************************************************************/

int main() {
	static const char *ip = "127.0.0.1";
	static const std::uint16_t port = 44550;
	static const char *avail = ""
		"\tpwd\n"
		"\tmkdir <dir name>\n"
		"\ttouch <file name>\n"
		"\trm    <file|dir name>\n"
		"\tls    [dir name]\n"
		"\tcd    <dir name>\n"
	;

	std::cout << "help:" << std::endl << avail << std::endl;

	YARMI_TRY(main_scope) {
		boost::asio::io_service ios;
		client c(ios);
		c.connect(ip, port);
		c.start();

		ios.post([&c]() {c.read_cmd();});

		ios.run();
	} YARMI_CATCH_LOG(main_scope, std::cerr, return 1;)

	return 0;
}

/***************************************************************************/
