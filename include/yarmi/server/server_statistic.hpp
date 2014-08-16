
// Copyright (c) 2013,2014, niXman (i dotty nixman doggy gmail dotty com)
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

#ifndef _yarmi__server__server_statistic_hpp
#define _yarmi__server__server_statistic_hpp

#include <cstdint>
#include <iosfwd>
#include <ctime>

namespace yarmi {

/***************************************************************************/

struct server_statistic {
	server_statistic()
		:uptime(0)
		,connections(0)
		,readed(0)
		,writen(0)
		,read_rate(0)
		,write_rate(0)
		,read_ops(0)
		,write_ops(0)
		,write_queue_size(0)
		,data_memory(0)
		,virt_memory(0)
		,user_cpu(0)
		,system_cpu(0)
		,total_cpu(0)
	{}

	void print(std::ostream &os) const;
	void reset();

	std::time_t		datetime;
	std::size_t		uptime;
	std::size_t		connections;
	std::uint64_t	readed;
	std::uint64_t	writen;
	std::size_t		read_rate;
	std::size_t		write_rate;
	std::size_t		read_ops;
	std::size_t		write_ops;
	std::size_t		write_queue_size;
	std::size_t		data_memory;
	std::size_t		virt_memory;
	std::size_t		user_cpu;
	std::size_t		system_cpu;
	std::size_t		total_cpu;
};

/***************************************************************************/

} // ns yarmi

#endif // _yarmi__server__server_statistic_hpp
