
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

#ifndef _yarmi__session_base_hpp
#define _yarmi__session_base_hpp

#include <yarmi/yarmi.hpp>
#include <yarmi/global_context_base.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <cassert>
#include <memory>

namespace yarmi {

/***************************************************************************/

struct session_base: std::enable_shared_from_this<session_base> {
	typedef std::shared_ptr<session_base> session_ptr;

	template<typename Impl, template<typename> class GC, typename D>
	static session_ptr create(boost::asio::io_service &ios, GC<Impl> &gc, D del) {
		Impl *impl = new Impl(ios, gc);
		session_ptr ptr;
		try {
			ptr.reset(impl, del);
		} catch(...) {
			delete impl;
		}
		return ptr;
	}

	session_base(boost::asio::io_service &ios);
	virtual ~session_base();

	boost::asio::ip::tcp::socket&
	get_socket();

	void start();
	void stop();
	void close();

	void send(const yas::shared_buffer &buffer);

	virtual void on_connected() = 0;
	virtual void on_disconnected() = 0;
	virtual void on_received(const char *ptr, std::size_t size) = 0;

	void set_on_destruction(bool flag);
	bool get_on_destruction() const;

private:
	struct impl;
	impl *pimpl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__session_base_hpp
