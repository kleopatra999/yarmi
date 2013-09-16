
#ifndef _yarmi__client_hpp
#define _yarmi__client_hpp

#include <boost/noncopyable.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <cstdint>
#include <memory>

namespace yarmi {

/***************************************************************************/

template<typename Invoker>
struct client_base: private boost::noncopyable {
	enum { header_size = sizeof(std::uint32_t)+YARMI_IARCHIVE_TYPE::_header_size };

	client_base(boost::asio::io_service &ios, Invoker *invoker)
		:socket(ios)
		,invoker(invoker)
	{}

	void connect(const std::string &ip, const std::uint16_t port) {
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
		boost::system::error_code ec;
		socket.connect(ep, ec);

		if ( ec ) throw std::runtime_error("client_base::on_connect(): "+ec.message());
		else on_connect();
	}
	void disconnect() {
		boost::system::error_code ec;
		socket.close(ec);
	}

	void send(std::shared_ptr<char> buffer, std::size_t size) {
		boost::asio::async_write(
			 socket
			,boost::asio::buffer(buffer.get(), size)
			,[this, buffer](const boost::system::error_code &ec, std::size_t) {
				if ( ec ) throw std::runtime_error("client_base send callback: "+ec.message());
			}
		);
	}

	virtual void on_yarmi_error(yas::uint8_t call_id, yas::uint8_t version_id, const std::string &msg) {
		std::cerr << "client_base::on_yarmi_error(" << (int)call_id << ", " << (int)version_id << "): " << msg << std::endl;
	}

private:
	void on_connect() {
		std::shared_ptr<char> header_buffer(new char[header_size], [](char *ptr){delete []ptr;});
		boost::asio::async_read(
			 socket
			,boost::asio::buffer(header_buffer.get(), header_size)
			,[this, header_buffer](const boost::system::error_code &ec, std::size_t) {
				if ( ! ec ) on_header_readed(header_buffer);
				else throw std::runtime_error("on_on_header_readed(): "+ec.message());
			}
		);
	}

	void on_header_readed(std::shared_ptr<char> header_buffer) {
		YARMI_IARCHIVE_TYPE ia(header_buffer.get(), header_size);
		std::uint32_t body_length = 0;
		ia & body_length;

		std::shared_ptr<char> body_buffer(new char[body_length], [](char *ptr){delete []ptr;});
		boost::asio::async_read(
			 socket
			,boost::asio::buffer(body_buffer.get(), body_length)
			,[this, body_length, body_buffer](const boost::system::error_code &ec, std::size_t) {
				if ( ! ec ) on_body_readed(body_buffer, body_length);
				else throw std::runtime_error("on_body_readed(): "+ec.message());
			}
		);
	}

	void on_body_readed(std::shared_ptr<char> body_buffer, std::size_t body_length) {
		invoker->invoke(body_buffer.get(), body_length);
		on_connect();
	}

private:
	boost::asio::ip::tcp::socket socket;
	Invoker *invoker;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__client_hpp
