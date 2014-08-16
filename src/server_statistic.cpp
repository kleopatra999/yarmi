
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

#include <yarmi/server/server_statistic.hpp>

#include <boost/format.hpp>

namespace yarmi {

/***************************************************************************/

std::string format_size(std::uint64_t fs) {
	static const std::uint64_t KB = 1024;
	static const std::uint64_t MB = KB*KB;
	static const std::uint64_t GB = MB*KB;
	static const std::uint64_t TB = GB*KB;

	if ( fs/TB ) {
		return (boost::format("%.2f Tb") % ((double)fs/TB)).str();
	} else if ( fs/GB ) {
		return (boost::format("%.2f Gb") % ((double)fs/GB)).str();
	} else if ( fs/MB ) {
		return (boost::format("%.2f Mb") % ((double)fs/MB)).str();
	} else if ( fs/KB ) {
		return (boost::format("%.2f Kb") % ((double)fs/KB)).str();
	}

	return (boost::format("%d B") % fs).str();
}

static std::string datetime_as_string(const std::time_t time) {
	struct tm *tm = 0;
	char buf[128] = "\0";

	tm = std::localtime(&time);
	std::strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", tm);

	return buf;
}

/***************************************************************************/

void server_statistic::print(std::ostream &os) const {
	static const char *fmt =
"datetime        : %s\n"
"uptime          : %04d:%02d:%02d\n"
"connections     : %d\n"
"total readed    : %d\n"
"total writen    : %d\n"
"read rate       : %d/sec\n"
"write rate      : %d/sec\n"
"read ops        : %d/sec\n"
"write ops       : %d/sec\n"
"send queue size : %d\n"
"data memory     : %d\n"
"virtual memory  : %d\n"
"CPU user   usage: %d%%\n"
"CPU system usage: %d%%\n"
"CPU total  usage: %d%%"
;

	os
	<< boost::format(fmt)
		% datetime_as_string(datetime)
		% (uptime/(60*60)) % ((uptime/(60))%60) % (uptime%60)
		% connections
		% format_size(readed)
		% format_size(writen)
		% format_size(read_rate)
		% format_size(write_rate)
		% read_ops
		% write_ops
		% write_queue_size
		% format_size(data_memory)
		% format_size(virt_memory)
		% user_cpu
		% system_cpu
		% total_cpu
	;
}

/***************************************************************************/

void server_statistic::reset() {
	read_rate = 0;
	write_rate = 0;
	read_ops = 0;
	write_ops = 0;
}

/***************************************************************************/

} // ns yarmi
