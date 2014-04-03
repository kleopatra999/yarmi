namespace yarmi {
template<typename Impl ,typename IO = Impl>
struct client_invoker {
	using id_type = decltype(::yarmi::detail::fnv1a_32(""));

	client_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}

private:
	static constexpr const char *_meta_requests_names[] = {
		 "yarmi ::server_invoker::on_ping (std::string)"
		,0
	};
	enum class _meta_requests_ids: id_type {
		ping_0 = ::yarmi::detail::fnv1a_32(_meta_requests_names[0])
	};
	static constexpr const char *_meta_handlers_names[] = {
		 "yarmi ::client_invoker::on_pong (std::string)"
		,0
	};
	enum class _meta_handlers_ids: id_type {
		on_pong_0 = ::yarmi::detail::fnv1a_32(_meta_handlers_names[0])
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
			call_id == static_cast<id_type>(_meta_handlers_ids::on_pong_0)
				? _meta_handlers_names[0]
				: 0
		);
	}
	static constexpr bool meta_has_handler(const id_type call_id) {
		return meta_handler_name(call_id) != 0;
	}

	void ping(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::ping_0)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}

	bool invoke(const id_type call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case static_cast<id_type>(_meta_handlers_ids::on_pong_0): {
				std::string arg0;
				ia & arg0;
				impl.on_pong(arg0);
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
template<typename Impl ,typename IO = Impl>
struct server_invoker {
	using id_type = decltype(::yarmi::detail::fnv1a_32(""));

	server_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}

private:
	static constexpr const char *_meta_requests_names[] = {
		 "yarmi ::client_invoker::on_pong (std::string)"
		,0
	};
	enum class _meta_requests_ids: id_type {
		pong_0 = ::yarmi::detail::fnv1a_32(_meta_requests_names[0])
	};
	static constexpr const char *_meta_handlers_names[] = {
		 "yarmi ::server_invoker::on_ping (std::string)"
		,0
	};
	enum class _meta_handlers_ids: id_type {
		on_ping_0 = ::yarmi::detail::fnv1a_32(_meta_handlers_names[0])
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
			call_id == static_cast<id_type>(_meta_handlers_ids::on_ping_0)
				? _meta_handlers_names[0]
				: 0
		);
	}
	static constexpr bool meta_has_handler(const id_type call_id) {
		return meta_handler_name(call_id) != 0;
	}

	void pong(const std::string &arg0) {
		yas::mem_ostream os, os2;
		yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header), pa(os2);
		oa & static_cast<id_type>(_meta_requests_ids::pong_0)
			& arg0;
		pa & os.get_intrusive_buffer();
		io.send(os2.get_shared_buffer());
	}

	bool invoke(const id_type call_id, yas::binary_iarchive<yas::mem_istream> &ia) {
		switch ( call_id ) {
			case static_cast<id_type>(_meta_handlers_ids::on_ping_0): {
				std::string arg0;
				ia & arg0;
				impl.on_ping(arg0);
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
