
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

#ifndef __yarmi__remote_fs__user_context_hpp
#define __yarmi__remote_fs__user_context_hpp

#include "protocol.hpp"

#include <yarmi/server/session.hpp>

/***************************************************************************/

template<typename>
struct global_context;

struct user_context: yarmi::session, yarmi::server_side<user_context> {
	user_context(const yarmi::socket_ptr &socket, yarmi::server_base &sb, global_context<user_context> &);
	virtual ~user_context();

	void on_connected();
	void on_disconnected();
	void on_received(const yarmi::call_id_type call_id, const yarmi::buffer_pair &buffer);

	void on_pwd();
	void on_mkdir(const std::string &);
	void on_touch(const std::string &);
	void on_rm(const std::string &);
	void on_ls(const std::string &);
	void on_cd(const std::string &);

private:
	struct impl;
	impl *pimpl;
};

/***************************************************************************/

#endif // __yarmi__remote_fs__user_context_hpp
