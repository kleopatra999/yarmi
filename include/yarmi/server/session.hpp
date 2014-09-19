
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

#ifndef _yarmi__server__session_base_hpp
#define _yarmi__server__session_base_hpp

#include <yarmi/yarmi_fwd.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace yarmi {

struct server_base;

/***************************************************************************/

struct session: std::enable_shared_from_this<session> {
	friend struct server_base;

	session(const socket_ptr &socket, server_base &sb);
	virtual ~session();

	typename socket_ptr::element_type& get_socket();

	void start();
	void stop();
	void stop(boost::system::error_code &ec);
	void close();
	void close(boost::system::error_code &ec);

	void send(const buffer_pair &buffer);

	virtual buffer_pair on_send(const buffer_pair &buffer);
	virtual void on_connected() {}
	virtual void on_disconnected() {}
	virtual void on_received(call_id_type call_id, const buffer_pair &buffer) = 0;

	// for checking
	bool on_destruction_state() const;

private:
	// for private use only
	void set_on_destruction_state();

private:
	struct impl;
	impl *pimpl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__server__session_base_hpp
