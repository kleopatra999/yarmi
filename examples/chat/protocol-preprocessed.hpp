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
			case ::yarmi::detail::fnv1a_32( "yarmi ::client_invoker::on_registration (std::string, std::string)" ):
				return "yarmi ::client_invoker::on_registration (std::string, std::string)";
			case ::yarmi::detail::fnv1a_32( "yarmi ::client_invoker::on_activation (std::string)" ):
				return "yarmi ::client_invoker::on_activation (std::string)";
			case ::yarmi::detail::fnv1a_32( "yarmi ::client_invoker::on_login (std::string)" ):
				return "yarmi ::client_invoker::on_login (std::string)";
			case ::yarmi::detail::fnv1a_32( "yarmi ::client_invoker::on_logout (std::string)" ):
				return "yarmi ::client_invoker::on_logout (std::string)";
			case ::yarmi::detail::fnv1a_32( "yarmi ::client_invoker::on_users_online (std::vector<std::string>)" ):
				return "yarmi ::client_invoker::on_users_online (std::vector<std::string>)";
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
	
	void registration( const std::string &arg0 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_registration (std::string)");
		oa & call_id
			& arg0;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void activation( const std::string &arg0 , const std::string &arg1 , const std::string &arg2 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_activation (std::string, std::string, std::string)");
		oa & call_id
			& arg0
			& arg1
			& arg2;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void login( const std::string &arg0 , const std::string &arg1 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_login (std::string, std::string)");
		oa & call_id
			& arg0
			& arg1;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void logout() {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_logout ()");
		oa & call_id;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void users_online() {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_users_online ()");
		oa & call_id;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void users_online( const std::string &arg0 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_users_online (std::string)");
		oa & call_id
			& arg0;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	bool invoke(const std::uint32_t call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_registration (std::string, std::string)"): {
				std::string arg0;
				std::string arg1;
				ia & arg0
					& arg1;
				impl.on_registration(arg0, arg1);
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_activation (std::string)"): {
				std::string arg0;
				ia & arg0;
				impl.on_activation(arg0);
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_login (std::string)"): {
				std::string arg0;
				ia & arg0;
				impl.on_login(arg0);
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_logout (std::string)"): {
				std::string arg0;
				ia & arg0;
				impl.on_logout(arg0);
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_users_online (std::vector<std::string>)"): {
				std::vector<std::string> arg0;
				ia & arg0;
				impl.on_users_online(arg0);
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
			case ::yarmi::detail::fnv1a_32( "yarmi ::server_invoker::on_registration (std::string)" ):
				return "yarmi ::server_invoker::on_registration (std::string)";
			case ::yarmi::detail::fnv1a_32( "yarmi ::server_invoker::on_activation (std::string, std::string, std::string)" ):
				return "yarmi ::server_invoker::on_activation (std::string, std::string, std::string)";
			case ::yarmi::detail::fnv1a_32( "yarmi ::server_invoker::on_login (std::string, std::string)" ):
				return "yarmi ::server_invoker::on_login (std::string, std::string)";
			case ::yarmi::detail::fnv1a_32( "yarmi ::server_invoker::on_logout ()" ):
				return "yarmi ::server_invoker::on_logout ()";
			case ::yarmi::detail::fnv1a_32( "yarmi ::server_invoker::on_users_online ()" ):
				return "yarmi ::server_invoker::on_users_online ()";
			case ::yarmi::detail::fnv1a_32( "yarmi ::server_invoker::on_users_online (std::string)" ):
				return "yarmi ::server_invoker::on_users_online (std::string)";
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
	
	void registration( const std::string &arg0 , const std::string &arg1 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_registration (std::string, std::string)");
		oa & call_id
			& arg0
			& arg1;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void activation( const std::string &arg0 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_activation (std::string)");
		oa & call_id
			& arg0;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void login( const std::string &arg0 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_login (std::string)");
		oa & call_id
			& arg0;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void logout( const std::string &arg0 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_logout (std::string)");
		oa & call_id
			& arg0;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void users_online( const std::vector<std::string> &arg0 ) {
		yas::mem_ostream os;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
		constexpr std::uint32_t call_id = ::yarmi::detail::fnv1a_32("yarmi ::client_invoker::on_users_online (std::vector<std::string>)");
		oa & call_id
			& arg0;
		yas::mem_ostream os2;
		yas::binary_oarchive<yas::mem_ostream> pa(os2);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	
	bool invoke(const std::uint32_t call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_registration (std::string)"): {
				std::string arg0;
				ia & arg0;
				impl.on_registration(arg0);
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_activation (std::string, std::string, std::string)"): {
				std::string arg0;
				std::string arg1;
				std::string arg2;
				ia & arg0
					& arg1
					& arg2;
				impl.on_activation(arg0, arg1, arg2);
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_login (std::string, std::string)"): {
				std::string arg0;
				std::string arg1;
				ia & arg0
					& arg1;
				impl.on_login(arg0, arg1);
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_logout ()"): {
				impl.on_logout();
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_users_online ()"): {
				impl.on_users_online();
				return true;
			}
			case ::yarmi::detail::fnv1a_32("yarmi ::server_invoker::on_users_online (std::string)"): {
				std::string arg0;
				ia & arg0;
				impl.on_users_online(arg0);
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
