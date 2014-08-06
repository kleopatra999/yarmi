
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

#ifndef _yarmi__server_base_hpp
#define _yarmi__server_base_hpp

#include <yarmi/server_statistic.hpp>

#include <boost/noncopyable.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <functional>

namespace yarmi {

/***************************************************************************/

struct server_config {
	server_config()
		:ip("127.0.0.1")
		,port(44550)
		,max_connections(10000)
		,max_recv_size(1024*1024*10)
		,recv_timeout(10)
		,send_timeout(10)
	{}

	std::string ip;
	std::uint16_t port;
	std::size_t max_connections;
	std::size_t max_recv_size;
	std::size_t recv_timeout;
	std::size_t send_timeout;
};

/***************************************************************************/

struct session_base;
struct global_context_base;
struct server_statistic;

struct server_base: boost::noncopyable {
	friend struct session_base;

	using connection_pred_type   = std::function<bool(const boost::asio::ip::tcp::endpoint &)>;
	using error_handler_type     = std::function<void(const std::string &)>;
	using statistic_handler_type = std::function<void(const server_statistic &)>;
	using session_factory_type   = std::function<session_base*()>;

	server_base(
		 boost::asio::io_service &ios
		,const server_config &config
		,global_context_base &gcb
		,connection_pred_type connection_pred
		,error_handler_type error_handler
		,statistic_handler_type stat_handler
		,session_factory_type session_factory
	);
	virtual ~server_base();

	boost::asio::io_service&
	get_io_service();

	const server_config& get_config() const;

	void start();
	void stop();
	void stop_accept();

private:
	server_statistic& get_server_statistic();
	const error_handler_type& get_error_handler() const;

private:
	struct impl;
	impl *pimpl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__server_base_hpp
