namespace yarmi {

template<typename Impl, typename IO = Impl>
struct client_invoker {
	client_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}
	
	void yarmi_error(const std::uint8_t &arg0, const std::uint8_t &arg1, const std::string &arg2) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(0)
			& static_cast<std::uint8_t>(0)
			& arg0
			& arg1
			& arg2;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void registration(const std::string &arg0) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(1)
			& static_cast<std::uint8_t>(0)
			& arg0;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void activation(const std::string &arg0, const std::string &arg1, const std::string &arg2) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(2)
			& static_cast<std::uint8_t>(0)
			& arg0
			& arg1
			& arg2;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void login(const std::string &arg0, const std::string &arg1) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(3)
			& static_cast<std::uint8_t>(0)
			& arg0
			& arg1;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void logout() {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(4)
			& static_cast<std::uint8_t>(0);
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void users_online() {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(5)
			& static_cast<std::uint8_t>(0);
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void users_online(const std::string &arg0) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(5)
			& static_cast<std::uint8_t>(1)
			& arg0;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void invoke(const char *ptr, std::size_t size) {
		std::uint8_t call_id, call_version;
		static const char* names[] = {
			 "yarmi_error"
			,"registration"
			,"activation"
			,"login"
			,"logout"
			,"users_online"
		};
		static const std::uint8_t versions[] = { 0, 0, 0, 0, 0, 0 };
		
		try {
			yas::binary_mem_iarchive ia(ptr, size, yas::no_header);
			ia & call_id
				& call_version;
			if ( call_id < 0 || call_id > 5 ) {
				char errstr[1024] = {0};
				std::snprintf(
					 errstr
					,sizeof(errstr)
					,"%s::%s(): bad call_id %d"
					,"client_invoker"
					,__FUNCTION__
					,static_cast<int>(call_id)
				);
				throw std::runtime_error(errstr);
			}
			if ( call_version > versions[call_id] ) {
				char errstr[1024] = {0};
				std::snprintf(
					 errstr
					,sizeof(errstr)
					,"%s::%s(): bad call_version %d for call_id %d(%s::%s())"
					,"client_invoker"
					,__FUNCTION__
					,static_cast<int>(call_version)
					,static_cast<int>(call_id)
					,"client_invoker"
					,names[call_id]
				);
				throw std::runtime_error(errstr);
			}
			
			switch ( call_id ) {
				case 0: {
					std::uint8_t arg0;
					std::uint8_t arg1;
					std::string arg2;
					ia & arg0
						& arg1
						& arg2;
					impl.on_yarmi_error( arg0 , arg1 , arg2);
				};
				break;
				case 1: {
					std::string arg0;
					std::string arg1;
					ia & arg0
						& arg1;
					impl.on_registration(arg0, arg1);
				};
				break;
				case 2: {
					std::string arg0;
					ia & arg0;
					impl.on_activation(arg0);
				};
				break;
				case 3: {
					std::string arg0;
					ia & arg0;
					impl.on_login(arg0);
				};
				break;
				case 4: {
					std::string arg0;
					ia & arg0;
					impl.on_logout(arg0);
				};
				break;
				case 5: {
					std::vector<std::string> arg0;
					ia & arg0;
					impl.on_users_online(arg0);
				};
				break;
			}
		} catch (const std::exception &ex) {
			char errstr[1024] = {0};
			std::snprintf(
				 errstr
				,sizeof(errstr)
				,"std::exception is thrown when %s::%s() is called: '%s'"
				,"client_invoker"
				,names[call_id]
				,ex.what()
			);
			yarmi_error(call_id, call_version, errstr);
		} catch (...) {
			char errstr[1024] = {0}; std::snprintf(
				 errstr
				,sizeof(errstr)
				,"unknown exception is thrown when %s::%s() is called"
				,"client_invoker"
				,names[call_id]
			);
			yarmi_error(call_id, call_version, errstr);
		}
	}

private:
	Impl &impl;
	IO &io;
}; // struct client_invoker

template<typename Impl, typename IO = Impl>
struct server_invoker {
	server_invoker(Impl &impl, IO &io)
		:impl(impl)
		,io(io)
	{}
	
	void yarmi_error(const std::uint8_t &arg0, const std::uint8_t &arg1, const std::string &arg2) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(0)
			& static_cast<std::uint8_t>(0)
			& arg0
			& arg1
			& arg2;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void registration(const std::string &arg0, const std::string &arg1) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(1)
			& static_cast<std::uint8_t>(0)
			& arg0
			& arg1;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void activation(const std::string &arg0) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(2)
			& static_cast<std::uint8_t>(0)
			& arg0;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void login(const std::string &arg0) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(3)
			& static_cast<std::uint8_t>(0)
			& arg0;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void logout(const std::string &arg0) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(4)
			& static_cast<std::uint8_t>(0)
			& arg0;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	void users_online(const std::vector<std::string> &arg0) {
		yas::binary_mem_oarchive oa(yas::no_header);
		oa & static_cast<std::uint8_t>(5)
			& static_cast<std::uint8_t>(0)
			& arg0;
		yas::binary_mem_oarchive pa;
		pa & oa.get_intrusive_buffer();
		io.send(pa.get_shared_buffer());
	}
	
	void invoke(const char *ptr, std::size_t size) {
		std::uint8_t call_id, call_version;
		static const char* names[] = {
			 "yarmi_error"
			,"registration"
			,"activation"
			,"login"
			,"logout"
			,"users_online"
		};
		static const std::uint8_t versions[] = { 0, 0, 0, 0, 0, 1 };
		
		try {
			yas::binary_mem_iarchive ia(ptr, size, yas::no_header);
			ia & call_id
				& call_version;
			if ( call_id < 0 || call_id > 5 ) {
				char errstr[1024] = {0};
				std::snprintf(
					 errstr
					,sizeof(errstr)
					,"%s::%s(): bad call_id %d"
					,"server_invoker"
					,__FUNCTION__
					,static_cast<int>(call_id)
				);
				throw std::runtime_error(errstr);
			}
			if ( call_version > versions[call_id] ) {
				char errstr[1024] = {0};
				std::snprintf(
					 errstr
					,sizeof(errstr)
					,"%s::%s(): bad call_version %d for call_id %d(%s::%s())"
					,"server_invoker"
					,__FUNCTION__
					,static_cast<int>(call_version)
					,static_cast<int>(call_id)
					,"server_invoker"
					,names[call_id]
				);
				throw std::runtime_error(errstr);
			}
			
			switch ( call_id ) {
				case 0: {
					std::uint8_t arg0;
					std::uint8_t arg1;
					std::string arg2;
					ia & arg0
						& arg1
						& arg2;
					impl.on_yarmi_error(arg0, arg1, arg2);
				};
				break;
				case 1: {
					std::string arg0;
					ia & arg0;
					impl.on_registration(arg0);
				};
				break;
				case 2: {
					std::string arg0;
					std::string arg1;
					std::string arg2;
					ia & arg0
						& arg1
						& arg2;
					impl.on_activation(arg0, arg1, arg2);
				};
				break;
				case 3: {
					std::string arg0;
					std::string arg1;
					ia & arg0
						& arg1;
					impl.on_login(arg0, arg1);
				};
				break;
				case 4: {
					impl.on_logout();
				};
				break;
				case 5: {
					switch ( call_version ) {
						case 0: {
							impl.on_users_online();
						};
						break;
						case 1: {
							std::string arg0;
							ia & arg0;
							impl.on_users_online(arg0);
						};
						break;
					}
				};
				break;
			}
		} catch (const std::exception &ex) {
			char errstr[1024] = {0};
			std::snprintf(
				 errstr
				,sizeof(errstr)
				,"std::exception is thrown when %s::%s() is called: '%s'"
				,"server_invoker"
				,names[call_id]
				,ex.what()
			);
			yarmi_error(call_id, call_version, errstr);
		} catch (...) {
			char errstr[1024] = {0};
			std::snprintf(
				 errstr
				,sizeof(errstr)
				,"unknown exception is thrown when %s::%s() is called"
				,"server_invoker"
				,names[call_id]
			);
			yarmi_error(call_id, call_version, errstr);
		}
	}

private:
	Impl &impl;
	IO &io;
}; // struct server_invoker

} // ns yarmi
