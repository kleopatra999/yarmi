
#ifndef _yarmi__session_base_hpp
#define _yarmi__session_base_hpp

#include <yarmi/yarmi.hpp>
#include <yarmi/global_context_base.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <memory>

namespace yarmi {

/***************************************************************************/

struct session_base: std::enable_shared_from_this<session_base> {
	typedef std::shared_ptr<session_base> session_ptr;

	template<typename Impl, template<typename> class GC, typename D>
	static session_ptr create(boost::asio::io_service &ios, GC<Impl> &gc, D del) {
		session_ptr ptr(new Impl(ios, gc), del);
		return ptr;
	}

	session_base(boost::asio::io_service &ios, global_context_base &gcb);
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

	virtual void on_yarmi_error(yas::uint8_t call_id, yas::uint8_t version_id, const std::string &msg);

	void set_on_destruction(bool flag);
	bool get_on_destruction() const;

private:
	struct impl;
	impl *pimpl;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__session_base_hpp
