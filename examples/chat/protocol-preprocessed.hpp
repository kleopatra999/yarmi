namespace yarmi {
template<typename Impl, typename IO = Impl>
struct client_invoker {
	using id_type = decltype(::yarmi::detail::fnv1a_32(""));

	client_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}

private:
	static constexpr const char *_meta_requests_names[] = {
		 "yarmi ::server_invoker::on_registration (std::string)"
		,"yarmi ::server_invoker::on_activation (std::string, std::string, std::string)"
		,"yarmi ::server_invoker::on_login (std::string, std::string)"
		,"yarmi ::server_invoker::on_logout ()"
		,"yarmi ::server_invoker::on_users_online ()"
		,"yarmi ::server_invoker::on_users_online (std::string)"
		,0
	};
	enum class _meta_requests_ids: id_type {
		 registration_0 = ::yarmi::detail::fnv1a_32(_meta_requests_names[0])
		,activation_1   = ::yarmi::detail::fnv1a_32(_meta_requests_names[1])
		,login_2        = ::yarmi::detail::fnv1a_32(_meta_requests_names[2])
		,logout_3       = ::yarmi::detail::fnv1a_32(_meta_requests_names[3])
		,users_online_4 = ::yarmi::detail::fnv1a_32(_meta_requests_names[4])
		,users_online_5 = ::yarmi::detail::fnv1a_32(_meta_requests_names[5])
	};
	static constexpr const char *_meta_handlers_names[] = {
		 "yarmi ::client_invoker::on_registration (std::string, std::string)"
		,"yarmi ::client_invoker::on_activation (std::string)"
		,"yarmi ::client_invoker::on_login (std::string)"
		,"yarmi ::client_invoker::on_logout (std::string)"
		,"yarmi ::client_invoker::on_users_online (std::vector<std::string>)"
		,0
	};
	enum class _meta_handlers_ids: id_type {
		 on_registration_0 = ::yarmi::detail::fnv1a_32(_meta_handlers_names[0])
		,on_activation_1   = ::yarmi::detail::fnv1a_32(_meta_handlers_names[1])
		,on_login_2        = ::yarmi::detail::fnv1a_32(_meta_handlers_names[2])
		,on_logout_3       = ::yarmi::detail::fnv1a_32(_meta_handlers_names[3])
		,on_users_online_4 = ::yarmi::detail::fnv1a_32(_meta_handlers_names[4])
	};

public:
	static constexpr const char** meta_requests() {
		return _meta_requests_names;
	}
	static constexpr std::size_t meta_requests_count() {
		return (sizeof(_meta_requests_names)/sizeof(_meta_requests_names[0]))-1;
	}
	static constexpr const char** meta_handlers() {
		return _meta_handlers_names;
	}
	static constexpr std::size_t meta_handlers_count() {
		return (sizeof(_meta_handlers_names)/sizeof(_meta_handlers_names[0]))-1;
	}
	static constexpr const char* meta_handler_name(const id_type call_id) {
		return (
			call_id == static_cast<id_type>(_meta_handlers_ids::on_registration_0)
				? _meta_handlers_names[0]
				: call_id == static_cast<id_type>(_meta_handlers_ids::on_activation_1)
					? _meta_handlers_names[1]
					: call_id == static_cast<id_type>(_meta_handlers_ids::on_login_2)
						? _meta_handlers_names[2]
						: call_id == static_cast<id_type>(_meta_handlers_ids::on_logout_3)
							? _meta_handlers_names[3]
							: call_id == static_cast<id_type>(_meta_handlers_ids::on_users_online_4)
								? _meta_handlers_names[4]
								: 0
		);
	}
	static constexpr bool meta_has_handler(const id_type call_id) {
		return meta_handler_name(call_id) != 0;
	}

	void registration(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::registration_0)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void activation(const std::string &arg0, const std::string &arg1, const std::string &arg2) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::activation_1)
			& arg0
			& arg1
			& arg2;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void login(const std::string &arg0, const std::string &arg1) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::login_2)
			& arg0
			& arg1;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void logout() {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::logout_3);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void users_online() {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::users_online_4);
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void users_online(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::users_online_5)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}

	bool invoke(const id_type call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case static_cast<id_type>(_meta_handlers_ids::on_registration_0): {
				std::string arg0;
				std::string arg1;
				ia & arg0
					& arg1;
				impl.on_registration(arg0, arg1);
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_activation_1): {
				std::string arg0;
				ia & arg0;
				impl.on_activation(arg0);
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_login_2): {
				std::string arg0;
				ia & arg0;
				impl.on_login(arg0);
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_logout_3): {
				std::string arg0;
				ia & arg0;
				impl.on_logout(arg0);
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_users_online_4): {
				std::vector<std::string> arg0;
				ia & arg0;
				impl.on_users_online(arg0);
				return true;
			}
			default: return false;
		}
	}

	bool invoke(const char *ptr, const std::size_t size, id_type *cid = 0) {
		id_type call_id = 0;
		yas::mem_istream is(ptr, size);
		yas::binary_iarchive<yas::mem_istream> ia(is, yas::no_header);
		ia & call_id;
		if ( cid )
			*cid = call_id;
		return invoke(call_id, ia);
	}

private:
	Impl &impl;
	IO &io;
};
} // ns yarmi

namespace yarmi {
template<typename Impl, typename IO = Impl>
struct server_invoker {
	using id_type = decltype(::yarmi::detail::fnv1a_32(""));

	server_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}

private:
	static constexpr const char *_meta_requests_names[] = {
		 "yarmi ::client_invoker::on_registration (std::string, std::string)"
		,"yarmi ::client_invoker::on_activation (std::string)"
		,"yarmi ::client_invoker::on_login (std::string)"
		,"yarmi ::client_invoker::on_logout (std::string)"
		,"yarmi ::client_invoker::on_users_online (std::vector<std::string>)"
		,0
	};
	enum class _meta_requests_ids: id_type {
		 registration_0 = ::yarmi::detail::fnv1a_32(_meta_requests_names[0])
		,activation_1   = ::yarmi::detail::fnv1a_32(_meta_requests_names[1])
		,login_2        = ::yarmi::detail::fnv1a_32(_meta_requests_names[2])
		,logout_3       = ::yarmi::detail::fnv1a_32(_meta_requests_names[3])
		,users_online_4 = ::yarmi::detail::fnv1a_32(_meta_requests_names[4])
	};
	static constexpr const char *_meta_handlers_names[] = {
		 "yarmi ::server_invoker::on_registration (std::string)"
		,"yarmi ::server_invoker::on_activation (std::string, std::string, std::string)"
		,"yarmi ::server_invoker::on_login (std::string, std::string)"
		,"yarmi ::server_invoker::on_logout ()"
		,"yarmi ::server_invoker::on_users_online ()"
		,"yarmi ::server_invoker::on_users_online (std::string)"
		,0
	};
	enum class _meta_handlers_ids: id_type {
		 on_registration_0 = ::yarmi::detail::fnv1a_32(_meta_handlers_names[0])
		,on_activation_1   = ::yarmi::detail::fnv1a_32(_meta_handlers_names[1])
		,on_login_2        = ::yarmi::detail::fnv1a_32(_meta_handlers_names[2])
		,on_logout_3       = ::yarmi::detail::fnv1a_32(_meta_handlers_names[3])
		,on_users_online_4 = ::yarmi::detail::fnv1a_32(_meta_handlers_names[4])
		,on_users_online_5 = ::yarmi::detail::fnv1a_32(_meta_handlers_names[5])
	};

public:
	static constexpr const char** meta_requests() {
		return _meta_requests_names;
	}
	static constexpr std::size_t meta_requests_count() {
		return (sizeof(_meta_requests_names)/sizeof(_meta_requests_names[0]))-1;
	}
	static constexpr const char** meta_handlers() {
		return _meta_handlers_names;
	}
	static constexpr std::size_t meta_handlers_count() {
		return (sizeof(_meta_handlers_names)/sizeof(_meta_handlers_names[0]))-1;
	}
	static constexpr const char* meta_handler_name(const id_type call_id) {
		return (
			call_id == static_cast<id_type>(_meta_handlers_ids::on_registration_0)
				? _meta_handlers_names[0]
				: call_id == static_cast<id_type>(_meta_handlers_ids::on_activation_1)
					? _meta_handlers_names[1]
					: call_id == static_cast<id_type>(_meta_handlers_ids::on_login_2)
						? _meta_handlers_names[2]
						: call_id == static_cast<id_type>(_meta_handlers_ids::on_logout_3)
							? _meta_handlers_names[3]
							: call_id == static_cast<id_type>(_meta_handlers_ids::on_users_online_4)
								? _meta_handlers_names[4]
								: call_id == static_cast<id_type>(_meta_handlers_ids::on_users_online_5)
									? _meta_handlers_names[5]
									: 0
		);
	}
	static constexpr bool meta_has_handler(const id_type call_id) {
		return meta_handler_name(call_id) != 0;
	}

	void registration(const std::string &arg0, const std::string &arg1) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::registration_0)
			& arg0
			& arg1;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void activation(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::activation_1)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void login(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::login_2)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void logout(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::logout_3)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}
	void users_online(const std::vector<std::string> &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::users_online_4)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}

	bool invoke(const id_type call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case static_cast<id_type>(_meta_handlers_ids::on_registration_0): {
				std::string arg0;
				ia & arg0;
				impl.on_registration(arg0);
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_activation_1): {
				std::string arg0;
				std::string arg1;
				std::string arg2;
				ia & arg0
					& arg1
					& arg2;
				impl.on_activation(arg0, arg1, arg2);
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_login_2): {
				std::string arg0;
				std::string arg1;
				ia & arg0
					& arg1;
				impl.on_login(arg0, arg1);
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_logout_3): {
				impl.on_logout();
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_users_online_4): {
				impl.on_users_online();
				return true;
			}
			case static_cast<id_type>(_meta_handlers_ids::on_users_online_5): {
				std::string arg0;
				ia & arg0;
				impl.on_users_online(arg0);
				return true;
			}
			default: return false;
		}
	}
	bool invoke(const char *ptr, const std::size_t size, id_type *cid = 0) {
		id_type call_id = 0;
		yas::mem_istream is(ptr, size);
		yas::binary_iarchive<yas::mem_istream> ia(is, yas::no_header);
		ia & call_id;
		if ( cid )
			*cid = call_id;
		return invoke(call_id, ia);
	}

private:
	Impl &impl;
	IO &io;
};
} // ns yarmi
