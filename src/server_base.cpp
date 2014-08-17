
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

#include <yarmi/server/handler_allocator.hpp>
#include <yarmi/server/make_preallocated_handler.hpp>
#include <yarmi/server/server_base.hpp>
#include <yarmi/server/session.hpp>
#include <yarmi/server/server_statistic.hpp>
#include <yarmi/server/global_context_base.hpp>
#include <yarmi/server/os_resources.hpp>

#include <yarmi/detail/throw/throw.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <ctime>

namespace yarmi {

/***************************************************************************/

struct server_base::impl {
	impl(
		 boost::asio::io_service &ios
		,const server_config &config
		,global_context_base &gcb
		,connection_pred_type connection_pred
		,error_handler_type error_handler
		,statistic_handler_type stat_handler
		,session_factory_type session_factory
	)
		:ios(ios)
		,config(config)
		,gcb(gcb)
		,allocator()
		,acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(config.ip), config.port))
		,stat_timer(ios)
		,connection_pred(connection_pred)
		,error_handler(error_handler)
		,stat_handler(stat_handler)
		,session_factory(session_factory)
		,start_time_in_seconds(std::time(0))
		,stat()
	{
		boost::system::error_code ec;
		on_timeout(ec);
		start_timer();
	}

	void stop() { acceptor.get_io_service().stop(); }
	void stop_accept() {
		boost::system::error_code ec;
		acceptor.cancel(ec);
	}

	void start() {
		yarmi::socket_ptr socket = std::make_shared<typename socket_ptr::element_type>(ios);
		acceptor.async_accept(
			 *socket
			,yarmi::make_preallocated_handler(
				 allocator
				,[this, socket](const boost::system::error_code &ec) { on_accepted(ec, socket); }
			)
		);
	}
	void on_accepted(const boost::system::error_code &ec, const yarmi::socket_ptr &socket) {
		if ( !ec ) {
			if ( gcb.sessions() == config.max_connections ) {
				error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("more connections than are set by 'max_connections'(%1%)", config.max_connections));

				/** start accepting next connection */
				start();
				return;
			}

			boost::system::error_code ec2;
			const boost::asio::ip::tcp::endpoint &ep = socket->remote_endpoint(ec2);
			if ( ec2 ) {
				error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("cannot get remote endpoint: \"%1%\"", ec2.message()));
			}

			if ( !connection_pred(ep) ) {
				error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("IP \"%1%\" is in backlist", ep.address().to_string()));
			} else {
				std::ostringstream os;
				yarmi::session_ptr session_ptr;

				YARMI_TRY(allocate_session_flag) {
					session_ptr.reset(session_factory(socket), [this](session *session_ptr){ session_deleter(session_ptr); });
				} YARMI_CATCH_LOG(allocate_session_flag, os,
					error_handler(os.str());
				);
				if ( allocate_session_flag ) {
					error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("stop accepting"));
					return;
				}

				YARMI_TRY(add_session_flag) {
					gcb.add_session(session_ptr.get());
				} YARMI_CATCH_LOG(add_session_flag, os,
					error_handler(os.str());
				);

				YARMI_TRY(on_connected_flag) {
					session_ptr->on_connected();
				} YARMI_CATCH_LOG(on_connected_flag, os,
					error_handler(os.str());
				);

				/** start session */
				session_ptr->start();
			}
		}

		/** start accepting next connection */
		start();
	}

	void session_deleter(session *session) {
		std::ostringstream os;

		session->set_on_destruction_state();

		YARMI_TRY(on_disconnected_flag) {
			session->on_disconnected();
		} YARMI_CATCH_LOG(on_disconnected_flag, os, error_handler(os.str());)

		if ( ! gcb.has_session(session) ) {
			error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("session \"%1%\" not in connected sessions list", session));
		} else {
			gcb.del_session(session);
		}

		YARMI_TRY(delete_session_flag) {
			delete session;
		} YARMI_CATCH_LOG(delete_session_flag, os, error_handler(os.str());)
	}

	/************************************************************************/

	void start_timer() {
		stat_timer.expires_from_now(boost::posix_time::seconds(1));
		stat_timer.async_wait([this](const boost::system::error_code &ec){on_timeout(ec);});
	}

	void on_timeout(const boost::system::error_code &ec) {
		stat.connections = gcb.sessions();
		const std::time_t t = std::time(0);
		stat.datetime = t;
		stat.uptime = t-start_time_in_seconds;

		detail::resources res = detail::get_resources_usage();
		stat.data_memory = res.resident_memory_usage;
		stat.virt_memory = res.virtual_memory_usage;
		stat.user_cpu = res.user_cpu_usage;
		stat.system_cpu = res.system_cpu_usage;
		stat.total_cpu = res.total_cpu_usage;

		if ( stat_handler )
			stat_handler(stat);

		stat.reset();

		if ( !ec )
			start_timer();
	}

	boost::asio::io_service &ios;
	const server_config &config;
	global_context_base &gcb;

	yarmi::handler_allocator<512> allocator;
	boost::asio::ip::tcp::acceptor acceptor;

	boost::asio::deadline_timer stat_timer;

	connection_pred_type connection_pred;
	error_handler_type error_handler;
	statistic_handler_type stat_handler;
	session_factory_type session_factory;

	const time_t start_time_in_seconds;
	server_statistic stat;
};

/***************************************************************************/

server_base::server_base(
	 boost::asio::io_service &ios
	,const server_config &config
	,global_context_base &gcb
	,connection_pred_type connection_pred
	,error_handler_type error_handler
	,statistic_handler_type stat_handler
	,session_factory_type session_factory
)
	:pimpl(new impl(ios, config, gcb, connection_pred, error_handler, stat_handler, session_factory))
{}

server_base::~server_base()
{ delete pimpl; }

boost::asio::io_service &server_base::get_io_service() { return pimpl->ios; }

const server_config &server_base::get_config() const { return pimpl->config; }

/***************************************************************************/

void server_base::start() { pimpl->start(); }
void server_base::stop() { pimpl->stop(); }

void server_base::stop_accept() { pimpl->stop_accept(); }

server_statistic &server_base::get_server_statistic() { return pimpl->stat; }

const server_base::error_handler_type &server_base::get_error_handler() const { return pimpl->error_handler; }

/***************************************************************************/

} // ns yarmi
