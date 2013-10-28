
// Copyright (c) 2013, niXman
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

#ifndef _yarmi__server_hpp
#define _yarmi__server_hpp

#include "session_base.hpp"

#include <boost/noncopyable.hpp>

#include <cstdint>

#include <string>
#include <sstream>
#include <unordered_set>

namespace yarmi {

/***************************************************************************/

template<typename UC, template<typename> class GC>
struct server: private boost::noncopyable {
private:
	static bool default_on_connected_predicate(const boost::asio::ip::tcp::endpoint &) {
		return true;
	}
	static void default_error_handler(const std::string &msg) {
		std::cerr << "YARMI: server: \"" << msg << "\"" << std::endl << std::flush;
	}

public:
	template<
		 typename CP = decltype(&default_on_connected_predicate)
		,typename EH = decltype(&default_error_handler)
	>
	server(
		 const std::string &ip
		,std::uint16_t port
		,boost::asio::io_service &ios
		,GC<UC> &gc
		,CP cp = &default_on_connected_predicate
		,EH eh = &default_error_handler
	)
		:acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port))
		,gc(gc)
		,cp(cp)
		,eh(eh)
	{}

	void start() {
		auto session = session_base::create<UC>(
			acceptor.get_io_service()
			,gc
			,std::bind(&server<UC, GC>::session_deleter, this, std::placeholders::_1)
		);

		acceptor.async_accept(
			 session->get_socket()
			,std::bind(&server<UC, GC>::on_accepted, this, std::placeholders::_1, session)
		);
	}

private:
	void session_deleter(session_base *session) {
		if ( ! gc.has_session(session) ) {
			std::ostringstream os;
			os << "session " << std::hex << session << " not in connected sessions list";
			eh(os.str());
		} else {
			gc.del_session(session);
		}

		session->set_on_destruction(true);

		try {
			session->on_disconnected();
		} catch (const std::exception &ex) {
			std::ostringstream os;
			os << "[exception] session->on_disconnected(): \"" << ex.what() << "\"";
			eh(os.str());
		}

		try {
			delete session;
		} catch (const std::exception &ex) {
			std::ostringstream os;
			os << "[exception] delete session: \"" << ex.what() << "\"";
			eh(os.str());
		}
	}

	void on_accepted(const boost::system::error_code &ec, session_base::session_ptr session) {
		if ( ! ec ) {
			boost::system::error_code ec2;
			const boost::asio::ip::tcp::endpoint &ep = session->get_socket().remote_endpoint(ec2);
			if ( ec2 ) {
				std::ostringstream os;
				os << "cannot get remote endpoint\"" << ec2.message() << "\"";
				eh(os.str());
			}

			if ( ! cp(ep) ) {
				std::ostringstream os;
				os << "IP \"" << ep.address().to_string() << "\" is in backlist";
				eh(os.str());
			} else {
				gc.add_session(session.get());

				try {
					session->on_connected();
				} catch (const std::exception &ex) {
					std::ostringstream os;
					os << "[exception] session->on_connected(): \"" << ex.what() << "\"";
					eh(os.str());
				}

				/** start session */
				session->start();
			}

			/** start accepting next connection */
			start();
		}
	}

private:
	boost::asio::ip::tcp::acceptor acceptor;
	GC<UC> &gc;

	std::function<bool(const boost::asio::ip::tcp::endpoint &)> cp;
	std::function<void(const std::string &)> eh;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__server_hpp
