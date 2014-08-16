
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

#include <boost/filesystem.hpp>

#include <vector>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <iostream>

#include <fnmatch.h>

/***************************************************************************/

struct user_context::impl {
	impl(global_context<user_context> &gc)
		:gc(gc)
		,cwd(gc.root())
	{}

	int fnmatch(const std::string &name) {
		if ( name.empty() ) return 0;
		const int res = ::fnmatch((gc.root() + "/*").c_str(), name.c_str(), 0);
		return (0==res ? 0 : errno);
	}

	global_context<user_context> &gc;
	std::string cwd;
};

/***************************************************************************/

user_context::user_context(const yarmi::socket_ptr &socket, yarmi::server_base &sb, global_context<user_context> &gc)
	:yarmi::session(socket, sb)
	,yarmi::server_side<user_context>(*this, *this)
	,pimpl(new impl(gc))
{}

user_context::~user_context()
{ delete pimpl; }

/***************************************************************************/

void user_context::on_connected() {}
void user_context::on_disconnected() {}

/***************************************************************************/

void user_context::on_received(const yarmi::buffer_pair &buffer) {
	yarmi::call_id_type call_id = 0;
	YARMI_TRY(invoke_flag) {
		if ( !yarmi::invoke(buffer, &call_id, *this) ) {
			std::cerr << "no handler for call_id=" << call_id << std::endl;
		}
	} YARMI_CATCH_LOG(invoke_flag, std::cerr)
}

/***************************************************************************/

void user_context::on_pwd() {
	pwd(0, "", pimpl->cwd);
}

void user_context::on_mkdir(const std::string &name) {
	int ec = pimpl->fnmatch(name);
	std::cout << "on_mkdir(): cwd=\"" << pimpl->cwd << "\", name=\"" << name << "\", ec=" << ec << std::endl;
	if ( ec ) {
		const std::string em = ::strerror(ec);
		mkdir(ec, em, ""); // >>>>>>>>>>>>>>>>>>>
	} else {
		boost::system::error_code ec;
		boost::filesystem::create_directories(pimpl->cwd + "/" + name, ec);
		if ( ec ) {
			mkdir(ec.value(), ec.message(), ""); // >>>>>>>>>>>>>>>>>>>
		} else {
			mkdir(0, "", ""); // >>>>>>>>>>>>>>>>>>>
		}
	}
}

void user_context::on_touch(const std::string &name) {
	int ec = pimpl->fnmatch(name);
	std::cout << "on_touch(): cwd=\"" << pimpl->cwd << "\", name=\"" << name << "\", ec=" << ec << std::endl;
	if ( ec ) {
		const std::string em = ::strerror(ec);
		touch(ec, em, ""); // >>>>>>>>>>>>>>>>>>>
	} else {
		std::ofstream file(pimpl->cwd + "/" + name);
		if ( !file ) {
			const int error = errno;
			const std::string em = ::strerror(error);
			touch(error, em, ""); // >>>>>>>>>>>>>>>>>>>
		} else {
			touch(0, "", ""); // >>>>>>>>>>>>>>>>>>>
		}
	}
}

void user_context::on_rm(const std::string &name) {
	int ec = pimpl->fnmatch(name);
	std::cout << "on_rm(): cwd=\"" << pimpl->cwd << "\", name=\"" << name << "\", ec=" << ec << std::endl;
	if ( ec ) {
		const std::string em = ::strerror(ec);
		rm(ec, em, ""); // >>>>>>>>>>>>>>>>>>>
	} else {
		boost::system::error_code ec;
		if ( !boost::filesystem::exists(pimpl->cwd + "/" + name, ec) ) {
			rm(ec.value(), ec.message(), ""); // >>>>>>>>>>>>>>>>>>>
		} else {
			boost::system::error_code ec;
			boost::filesystem::remove(pimpl->cwd + "/" + name, ec);
			rm(ec.value(), ec.message(), ""); // >>>>>>>>>>>>>>>>>>>
		}
	}
}

void user_context::on_ls(const std::string &name) {
	int ec = pimpl->fnmatch(name);
	std::cout << "on_ls(): cwd=\"" << pimpl->cwd << "\", name=\"" << name << "\", ec=" << ec << std::endl;
	if ( ec ) {
		const std::string em = ::strerror(ec);
		ls(ec, em, ""); // >>>>>>>>>>>>>>>>>>>
	} else {
		std::ostringstream os;
		std::vector<boost::filesystem::path> vec(
			 (boost::filesystem::directory_iterator((name.empty() ? pimpl->cwd : pimpl->cwd + "/" + name)))
			,(boost::filesystem::directory_iterator())
		);
		const std::size_t max = std::max_element(vec.begin(), vec.end()
			,[](const boost::filesystem::path &l, const boost::filesystem::path &r) {
				return l.string().length() < r.string().length();
			}
		)->string().length();

		auto cur = vec.begin(), end = vec.end();
		for ( ; cur != end; ++cur ) {
			boost::system::error_code ec;
			std::size_t size = 0;
			const bool isdir = boost::filesystem::is_directory(*cur, ec);
			if ( ec ) throw ec;
			const bool isfile = boost::filesystem::is_regular_file(*cur, ec);
			if ( ec ) throw ec;
			if ( isfile ) {
				size = boost::filesystem::file_size(*cur, ec);
				if ( ec ) throw ec;
			}

			os
			<< cur->string()
			<< std::string((max+1) - cur->string().length(), ' ')
			<< "| " << (isdir ? "d" : "f") << " | "
			<< (isfile ? std::to_string(size) : "---")
			<< std::endl;
		}

		ls(0, "", os.str()); // >>>>>>>>>>>>>>>>>>>
	}
}

void user_context::on_cd(const std::string &name) {
	int ec = pimpl->fnmatch(name);
	std::cout << "on_cd(): cwd=\"" << pimpl->cwd << "\", name=\"" << name << "\", ec=" << ec << std::endl;
	if ( ec ) {
		const std::string em = ::strerror(ec);
		cd(ec, em, ""); // >>>>>>>>>>>>>>>>>>>
	} else {
		boost::system::error_code ec;
		if ( -1 == ::chdir((pimpl->cwd + "/" + name).c_str()) ) {
			const int ec = errno;
			const std::string em = ::strerror(ec);
			cd(ec, em, ""); // >>>>>>>>>>>>>>>>>>>
		} else {
			pimpl->cwd = boost::filesystem::current_path(ec).string();
			cd(ec.value(), ec.message(), "");
		}
	}
}

/***************************************************************************/
