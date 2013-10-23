
#include <yarmi/session_base.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <list>
#include <functional>

namespace yarmi {

/***************************************************************************/

struct session_base::impl {
	enum { header_size = sizeof(yas::uint32_t)+YARMI_IARCHIVE_TYPE::_header_size };

	impl(boost::asio::io_service &ios)
		:socket(ios)
		,buffers()
		,in_process(false)
		,on_destruction(false)
	{}

	void read_header(session_base::session_ptr self) {
		boost::asio::async_read(
			 socket
			,boost::asio::buffer(header_buffer)
			,std::bind(
				&impl::header_readed
				,this
				,std::placeholders::_1
				,std::placeholders::_2
				,self
			)
		);
	}

	void header_readed(const boost::system::error_code &ec, std::size_t rd, session_base::session_ptr self) {
		if ( ec || rd != header_size ) {
			std::cerr << "YARMI: header read error: \"" << ec.message() << "\"" << std::endl;
			return;
		}

		YARMI_IARCHIVE_TYPE ia(header_buffer, header_size);
		std::uint32_t body_length = 0;
		ia & body_length;

		std::shared_ptr<char> body_buffer(new char[body_length], [](char *ptr){delete []ptr;});
		boost::asio::async_read(
			 socket
			,boost::asio::buffer(body_buffer.get(), body_length)
			,std::bind(
				&impl::body_readed
				,this
				,std::placeholders::_1
				,std::placeholders::_2
				,self
				,body_buffer
				,body_length
			)
		);
	}
	void body_readed(
		 const boost::system::error_code &ec
		,std::size_t rd
		,session_base::session_ptr self
		,std::shared_ptr<char> buffer
		,std::size_t buffer_size
	) {
		if ( ec || rd != buffer_size ) {
			std::cerr << "YARMI: body read error: \"" << ec.message() << "\"" << std::endl;
			return;
		}

		try {
			self->on_received(buffer.get(), buffer_size);
		} catch (const std::exception &ex) {
			std::cerr << "YARMI: exception is thrown when invoking: \"" << ex.what() << "\"" << std::endl;
		}

		read_header(self);
	}

	void send(session_base::session_ptr self) {
		if ( !in_process ) {
			in_process = true;

			yas::shared_buffer buffer = buffers.front();
			buffers.pop_front();

			boost::asio::async_write(
				 socket
				,boost::asio::buffer(buffer.data.get(), buffer.size)
				,std::bind(
					&impl::sent
					,this
					,std::placeholders::_1
					,std::placeholders::_2
					,self
					,buffer
				)
			);
		}
	}

	void sent(
		 const boost::system::error_code &ec
		,std::size_t wr
		,session_base::session_ptr self
		,yas::shared_buffer buffer
	) {
		if ( ec || wr != buffer.size ) {
			std::cerr << "YARMI: write error: \"" << ec.message() << "\"" << std::endl;
		}
		in_process = false;
		if ( ! buffers.empty() ) {
			send(self);
		}
	}

	boost::asio::ip::tcp::socket socket;

	/** buffers list */
	std::list<yas::shared_buffer> buffers;
	bool in_process;
	bool on_destruction;

	char header_buffer[header_size];
}; // struct session_base::impl

/***************************************************************************/

session_base::session_base(boost::asio::io_service &ios)
	:pimpl(new impl(ios))
{}

session_base::~session_base()
{ delete pimpl; }

/***************************************************************************/

boost::asio::ip::tcp::socket &session_base::get_socket()
{ return pimpl->socket; }

/***************************************************************************/

void session_base::start() {
	pimpl->read_header(shared_from_this());
}

/***************************************************************************/

void session_base::stop() { pimpl->socket.cancel(); }
void session_base::close() { pimpl->socket.close(); }

/***************************************************************************/

void session_base::send(const yas::shared_buffer &buffer) {
	if ( get_on_destruction() ) {
		std::cerr << "YARMI: cannot send data when session already in destruction state" << std::endl;
		return;
	}

	pimpl->buffers.push_back(buffer);

	pimpl->send(shared_from_this());
}

/***************************************************************************/

void session_base::on_yarmi_error(yas::uint8_t call_id, yas::uint8_t version_id, const std::string &msg) {
	std::cerr << "YARMI: on_yarmi_error(" << (int)call_id << ", " << (int)version_id << "): \"" << msg << "\"" << std::endl << std::flush;
}

/***************************************************************************/

void session_base::set_on_destruction(bool flag) { pimpl->on_destruction = flag; }
bool session_base::get_on_destruction() const { return pimpl->on_destruction; }

/***************************************************************************/

} // ns yarmi
