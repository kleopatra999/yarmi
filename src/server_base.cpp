
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

#include <yarmi/handler_allocator.hpp>
#include <yarmi/make_preallocated_handler.hpp>
#include <yarmi/server_base.hpp>
#include <yarmi/session_base.hpp>
#include <yarmi/server_statistic.hpp>
#include <yarmi/global_context_base.hpp>
#include <yarmi/throw.hpp>
#include <yarmi/os_resources.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <ctime>

namespace yarmi {

/***************************************************************************/

struct server_base::impl {
	impl(
		 boost::asio::io_service &ios
		,const std::string &ip
		,std::uint16_t port
		,global_context_base &gcb
		,connection_pred_type cp
		,error_handler_type eh
		,statistic_handler_type sh
		,session_factory_type sc
	)
		:allocator()
		,acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port))
		,timer(ios)
		,cp(cp)
		,eh(eh)
		,sh(sh)
		,sc(sc)
		,start_time_in_seconds(std::time(0))
		,gcb(gcb)
		,stat()
	{
		start_timer();
	}

	void start() {
		session_base *session_ptr = sc();
		session_base::session_ptr session(session_ptr, [this](session_base *session){ session_deleter(session); });

		acceptor.async_accept(
			 session->get_socket()
			,yarmi::make_preallocated_handler(
				 allocator
				,[this, session](const boost::system::error_code &ec) { on_accepted(ec, session); }
			)
		);
	}
	void stop() { acceptor.get_io_service().stop(); }
	void stop_accept() {
		boost::system::error_code ec;
		acceptor.cancel(ec);
	}

	void session_deleter(session_base *session) {
		std::ostringstream os;

		YARMI_TRY(on_disconnected_flag)
			session->on_disconnected();
		YARMI_CATCH_LOG(on_disconnected_flag, os, eh(os.str());)

		if ( ! gcb.has_session(session) ) {
			eh(YARMI_FORMAT_MESSAGE_AS_STRING("session \"%1%\" not in connected sessions list", session));
		} else {
			gcb.del_session(session);
		}

		YARMI_TRY(delete_session_flag)
			delete session;
		YARMI_CATCH_LOG(delete_session_flag, os, eh(os.str());)
	}

	void on_accepted(const boost::system::error_code &ec, session_base::session_ptr session) {
		if ( ! ec ) {
			boost::system::error_code ec2;
			const boost::asio::ip::tcp::endpoint &ep = session->get_socket().remote_endpoint(ec2);
			if ( ec2 ) {
				eh(YARMI_FORMAT_MESSAGE_AS_STRING("cannot get remote endpoint: \"%1%\"", ec2.message()));
			}

			if ( ! cp(ep) ) {
				eh(YARMI_FORMAT_MESSAGE_AS_STRING("IP \"%1%\" is in backlist", ep.address().to_string()));
			} else {
				std::ostringstream os;
				YARMI_TRY(add_session_flag)
					gcb.add_session(session.get());
				YARMI_CATCH_LOG(add_session_flag, os,
					eh(os.str());
				);

				YARMI_TRY(on_connected_flag)
					session->on_connected();
				YARMI_CATCH_LOG(on_connected_flag, os,
					eh(os.str());
				);

				/** start session */
				session->start();
			}

			/** start accepting next connection */
			start();
		}
	}

	/************************************************************************/

	static std::string datetime_as_string(const std::time_t time) {
		struct tm *tm = 0;
		char buf[128] = "\0";

		tm = std::localtime(&time);
		if ( !tm ) {
			YARMI_FORMAT_MESSAGE_AS_STRING("\"t\" is NULL");
		} else {
			std::strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", tm);
		}

		return buf;
	}

	void start_timer() {
		timer.expires_from_now(boost::posix_time::seconds(1));
		timer.async_wait([this](const boost::system::error_code &ec){on_timeout(ec);});
	}

	void on_timeout(const boost::system::error_code &ec) {
		stat.connections = gcb.sessions();
		std::time_t time = std::time(0);
		stat.seconds = time-start_time_in_seconds;
		stat.datetime = datetime_as_string(time);

		detail::memory_usage(&stat.virt_memory, &stat.data_memory);
		detail::cpu_usage(&stat.user_cpu, &stat.system_cpu);
		stat.total_cpu = stat.user_cpu + stat.system_cpu;

		if ( sh )
			sh(stat);

		stat.read_rate = 0;
		stat.write_rate = 0;
		stat.read_ops = 0;
		stat.write_ops = 0;
		stat.write_queue_size = 0;

		if ( !ec )
			start_timer();
	}

	yarmi::handler_allocator<512> allocator;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::deadline_timer timer;
	connection_pred_type cp;
	error_handler_type eh;
	statistic_handler_type sh;
	session_factory_type sc;
	const time_t start_time_in_seconds;
	global_context_base &gcb;
	server_statistic stat;
};

/***************************************************************************/

server_base::server_base(
	 boost::asio::io_service &ios
	,const std::string &ip
	,const std::uint16_t port
	,global_context_base &gcb
	,connection_pred_type cp
	,error_handler_type eh
	,statistic_handler_type sh
	,session_factory_type sc
)
	:pimpl(new impl(ios, ip, port, gcb, cp, eh, sh, sc))
{}

server_base::~server_base()
{ delete pimpl; }

boost::asio::io_service &server_base::get_io_service() { return pimpl->acceptor.get_io_service(); }

/***************************************************************************/

void server_base::start() { pimpl->start(); }
void server_base::stop() { pimpl->stop(); }

void server_base::stop_accept() { pimpl->stop_accept(); }

server_statistic &server_base::get_server_statistic() { return pimpl->stat; }

const server_base::error_handler_type &server_base::get_error_handler() const { return pimpl->eh; }

/***************************************************************************/

} // ns yarmi
