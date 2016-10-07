
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

#include <yarmi/server/session.hpp>
#include <yarmi/server/server_base.hpp>
#include <yarmi/server/global_context_base.hpp>
#include <yarmi/server/handler_allocator.hpp>
#include <yarmi/server/make_preallocated_handler.hpp>
#include <yarmi/serializers/binary_serializer_base.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <queue>
#include <functional>

namespace yarmi {

/***************************************************************************/

struct session::impl {
	enum { header_size = binary_serializer_base::header_size() };

	impl(socket sock, server_base &sb)
		:config(sb.get_config())
		,stat(sb.get_server_statistic())
		,sock(std::move(sock))
		,error_handler(sb.get_error_handler())
		,buffers()
		,write_in_process(false)
		,on_destruction_state(false)
	{}

	void read_header(const yarmi::session_ptr &self) {
		boost::asio::async_read(
			 sock
			,boost::asio::buffer(header_buffer)
			,yarmi::make_preallocated_handler(
				read_allocator
				,std::bind(
					&impl::header_readed
					,this
					,std::placeholders::_1
					,std::placeholders::_2
					,self
				)
			)
		);
	}
	void header_readed(
		 const boost::system::error_code &ec
		,const std::size_t rd
		,const yarmi::session_ptr &self
	) {
		++stat.read_ops;

		if ( ec == boost::asio::error::operation_aborted )
			return;

		if ( ec || rd != header_size ) {
			error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("header read error: \"%1%\"", ec.message()));
			return;
		}

		stat.readed += rd;
		stat.read_rate += rd;

		const std::pair<std::uint32_t, call_id_type> header = binary_serializer_base::unpack_header(header_buffer, header_size);
		if ( header.first == 0 ) {
			invoke(header.second, buffer_pair(), self);
			read_header(self);
		} else {
			if ( header.first > config.max_recv_size ) {
				error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("body size is too long: \"%1%\"", header.first));
				return;
			}

			read_body(header.first, header.second, self);
		}
	}
	void read_body(const std::size_t body_size, const call_id_type call_id, const yarmi::session_ptr &self) {
		::yarmi::buffer_pair buffer = allocate_buffer(body_size);
		boost::asio::async_read(
			 sock
			,boost::asio::buffer(buffer.first.get(), buffer.second)
			,yarmi::make_preallocated_handler(
				read_allocator
				,std::bind(
					&impl::body_readed
					,this
					,std::placeholders::_1
					,std::placeholders::_2
					,self
					,call_id
					,buffer
				)
			)
		);
	}
	void body_readed(
		 const boost::system::error_code &ec
		,const std::size_t rd
		,const yarmi::session_ptr &self
		,const yarmi::call_id_type call_id
		,const yarmi::buffer_pair &buffer
	) {
		++stat.read_ops;

		if ( ec == boost::asio::error::operation_aborted )
			return;

		if ( ec || rd != buffer.second ) {
			error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("body read error: \"%1%\"", ec.message()));
			return;
		}

		stat.readed += rd;
		stat.read_rate += rd;

		invoke(call_id, buffer, self);
		read_header(self);
	}
	void invoke(const call_id_type call_id, const buffer_pair &buffer, const session_ptr &self) {
		try {
			self->on_received(call_id, buffer);
		} catch (const std::exception &ex) {
			error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("invoke error: \"%1%\"", ex.what()));
		}
	}

	void send(const yarmi::session_ptr &self, const buffer_pair &buffer) {
		++stat.write_queue_size;

		if ( !write_in_process ) {
			write_in_process = true;
			boost::asio::async_write(
				 sock
				,boost::asio::buffer(buffer.first.get(), buffer.second)
				,yarmi::make_preallocated_handler(
					write_allocator
					,std::bind(
						&impl::sent
						,this
						,std::placeholders::_1
						,std::placeholders::_2
						,self
						,buffer
					)
				)
			);
		} else {
			buffers.push(std::move(buffer));
		}
	}

	void sent(
		 const boost::system::error_code &ec
		,const std::size_t wr
		,const yarmi::session_ptr &self
		,const buffer_pair &buffer
	) {
		write_in_process = false;

		--stat.write_queue_size;
		stat.writen += wr;
		stat.write_rate += wr;
		++stat.write_ops;

		if ( ec == boost::asio::error::operation_aborted )
			return;

		if ( ec || wr != buffer.second ) {
			error_handler(YARMI_FORMAT_MESSAGE_AS_STRING("write error: \"%1%\"", ec.message()));
			return;
		}

		if ( !buffers.empty() && !on_destruction_state ) {
			buffer_pair buffer = buffers.front();
			buffers.pop();

			send(self, buffer);
		}
	}

	yarmi::handler_allocator<512> read_allocator;
	yarmi::handler_allocator<512> write_allocator;

	const server_config &config;
	server_statistic &stat;
	socket sock;

	server_base::error_handler_type error_handler;

	/** buffers list */
	std::queue<buffer_pair> buffers;
	bool write_in_process;

	/** header */
	char header_buffer[header_size];

	/** on destruction state flag */
	bool on_destruction_state;
}; // struct session_base::impl

/***************************************************************************/

session::session(yarmi::socket sock, server_base &sb)
	:pimpl(new impl(std::move(sock), sb))
{}

session::~session() {
	boost::system::error_code ec;
	close(ec);

	delete pimpl;
}

/***************************************************************************/

typename socket_ptr::element_type& session::get_socket() { return pimpl->sock; }

/***************************************************************************/

void session::start() {
	pimpl->read_header(shared_from_this());
}

/***************************************************************************/

void session::stop() {
	boost::system::error_code ec;
	stop(ec);

	if ( ec )
		YARMI_THROW("ec=%1%, message=%2%", ec.value(), ec.message());
}

void session::stop(boost::system::error_code &ec) {
	pimpl->sock.cancel(ec);
}

void session::close() {
	boost::system::error_code ec;
	close(ec);

	if ( ec )
		YARMI_THROW("ec=%1%, message=%2%", ec.value(), ec.message());
}

void session::close(boost::system::error_code &ec) {
	pimpl->sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	if ( ec ) return;
	pimpl->sock.close(ec);
}

/***************************************************************************/

buffer_pair session::on_send(const buffer_pair &buffer) {
	return std::move(buffer);
}

void session::send(const buffer_pair &buffer) {
	BOOST_ASSERT_MSG(!pimpl->on_destruction_state, "session already ON_DESTRUCTION_STATE!");

	if ( !buffer.first.get() || !buffer.second )
		return;

	pimpl->send(shared_from_this(), on_send(buffer));
}

/***************************************************************************/

bool session::on_destruction_state() const { return pimpl->on_destruction_state; }

void session::set_on_destruction_state() {
	boost::system::error_code ec;
	pimpl->sock.cancel(ec);

	pimpl->on_destruction_state = true;
}

/***************************************************************************/

} // ns yarmi
