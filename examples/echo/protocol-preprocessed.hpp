namespace yarmi {
template<typename Impl, typename IO = Impl>
struct client_invoker {
	client_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}

private:
	static const char* yarmi_handler_name_impl(const std::uint32_t call_id) {
		switch ( call_id ) {
			case ::yarmi::detail::fnv1a_32( "yarmi ::client_invoker::on_pong (std::string)" ):
				return "yarmi ::client_invoker::on_pong (std::string)";
			default:
				return 0;
		}
	}

public:
	static const char* yarmi_handler_name(const std::uint32_t call_id) {
		const char *str = yarmi_handler_name_impl(call_id);
		return (str ? str : "unknown");
	}
	static bool yarmi_has_handler(const std::uint32_t call_id) {
		return yarmi_handler_name_impl(call_id) != 0;
	}
	
	void ping(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_ping (std::string)");
		oa & call_id
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	bool invoke(const std::uint32_t call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_pong (std::string)"): {
				std::string arg0;
				ia & arg0;
				impl.on_pong(arg0);
				return true;
			}
			default:
				return false;
		}
	}
	bool invoke(const char *ptr, const std::size_t size) {
		std::uint32_t call_id;
		yas::mem_istream is(ptr, size);
		yas::binary_iarchive<yas::mem_istream> ia(is, yas::no_header);
		ia & call_id;
		return invoke(call_id, ia);
	}

private:
	Impl &impl;
	IO &io;
}; // struct client_invoker
} // ns yarmi

namespace yarmi {
template<typename Impl, typename IO = Impl>
struct server_invoker {
	server_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}

private:
	static const char* yarmi_handler_name_impl(const std::uint32_t call_id) {
		switch ( call_id ) {
			case ::yarmi::detail::fnv1a_32( "yarmi ::server_invoker::on_ping (std::string)" ):
				return "yarmi ::server_invoker::on_ping (std::string)";
			default:
				return 0;
		}
	}
public:
	static const char* yarmi_handler_name(const std::uint32_t call_id) {
		const char *str = yarmi_handler_name_impl(call_id);
		return (str ? str : "unknown");
	}
	static bool yarmi_has_handler(const std::uint32_t call_id) {
		return yarmi_handler_name_impl(call_id) != 0;
	}
	
	void pong( const std::string &arg0 ) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_pong (std::string)");
		oa & call_id
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	
	bool invoke(const std::uint32_t call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_ping (std::string)"): {
				std::string arg0;
				ia & arg0;
				impl.on_ping(arg0);
				return true;
			}
			default:
				return false;
		}
	}
	bool invoke(const char *ptr, const std::size_t size) {
		std::uint32_t call_id;
		yas::mem_istream is(ptr, size);
		yas::binary_iarchive<yas::mem_istream> ia(is, yas::no_header);
		ia & call_id;
		return invoke(call_id, ia);
	}

private:
	Impl &impl;
	IO &io;
}; // struct server_invoker
} // ns yarmi
