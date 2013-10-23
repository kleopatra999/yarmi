
#ifndef _yarmi__client_base_hpp
#define _yarmi__client_base_hpp

#include <yas/detail/tools/buffers.hpp>

#include <boost/noncopyable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <cstdint>
#include <memory>
#include <list>
#include <functional>

namespace yarmi {

/***************************************************************************/

template<typename Invoker>
struct client_base: private boost::noncopyable {
	enum { header_size = sizeof(std::uint32_t)+YARMI_IARCHIVE_TYPE::_header_size };

	client_base(boost::asio::io_service &ios, Invoker &invoker)
		:socket(ios)
		,invoker(invoker)
		,buffers()
		,in_process(false)
	{}

	void connect(const std::string &ip, const std::uint16_t port) {
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
		boost::system::error_code ec;
		socket.connect(ep, ec);

		if ( ec )
			throw std::runtime_error("client_base::on_connect(): "+ec.message());
		else
			on_connected();
	}
	void disconnect() {
		boost::system::error_code ec;
		socket.close(ec);
	}

	void send(const yas::shared_buffer &buffer) {
		if ( ! in_process ) {
			in_process = true;

			boost::asio::async_write(
				 socket
				,boost::asio::buffer(buffer.data.get(), buffer.size)
				,std::bind(&client_base<Invoker>::sent, this, std::placeholders::_1, std::placeholders::_2, buffer)
			);
		} else {
			buffers.push_back(buffer);
		}
	}

	virtual void on_yarmi_error(yas::uint8_t call_id, yas::uint8_t version_id, const std::string &msg) {
		std::cerr << "client_base::on_yarmi_error(" << (int)call_id << ", " << (int)version_id << "): " << msg << std::endl;
	}

private:
	void on_connected() {
		boost::asio::async_read(
			 socket
			,boost::asio::buffer(header_buffer)
			,[this](const boost::system::error_code &ec, std::size_t) {
				if ( ! ec )
					on_header_readed();
				else
					throw std::runtime_error("on_on_header_readed(): "+ec.message());
			}
		);
	}

	void on_header_readed() {
		YARMI_IARCHIVE_TYPE ia(header_buffer, header_size);
		std::uint32_t body_length = 0;
		ia & body_length;

		std::shared_ptr<char> body_buffer(new char[body_length], [](char *ptr){delete []ptr;});
		boost::asio::async_read(
			 socket
			,boost::asio::buffer(body_buffer.get(), body_length)
			,[this, body_length, body_buffer](const boost::system::error_code &ec, std::size_t) {
				if ( ! ec )
					on_body_readed(body_buffer, body_length);
				else
					throw std::runtime_error("on_body_readed(): "+ec.message());
			}
		);
	}

	void on_body_readed(std::shared_ptr<char> body_buffer, std::size_t body_length) {
		invoker.invoke(body_buffer.get(), body_length);
		on_connected();
	}

	void sent(const boost::system::error_code &ec, std::size_t wr, yas::shared_buffer buffer) {
		in_process = false;

		if ( ec || wr != buffer.size )
			throw std::runtime_error("client_base send callback: "+ec.message());

		if ( ! buffers.empty() ) {
			yas::shared_buffer buf = buffers.front();
			buffers.pop_front();

			send(buf);
		}
	}

private:
	boost::asio::ip::tcp::socket socket;
	Invoker &invoker;

	std::list<yas::shared_buffer> buffers;
	bool in_process;

	char header_buffer[header_size];
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__client_base_hpp
