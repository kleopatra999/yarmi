
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

#include <yarmi/server_statistic.hpp>

#include <boost/format.hpp>

#include <ostream>

#include <sys/resource.h>
#include <sys/times.h>

namespace yarmi {

/***************************************************************************/

std::string format_size(std::uint64_t fs) {
	static const std::uint64_t KB = 1024;
	static const std::uint64_t MB = KB*KB;
	static const std::uint64_t GB = MB*KB;

	if ( (fs/GB) > 0 ) {
		return (boost::format("%.2f G") % ((double)fs/GB)).str();
	} else if ( (fs/MB) > 0 ) {
		return (boost::format("%.2f M") % ((double)fs/MB)).str();
	} else if ( (fs/KB) > 0 ) {
		return (boost::format("%.2f K") % ((double)fs/KB)).str();
	}

	return (boost::format("%d B")   % fs).str();
}

/***************************************************************************/

void server_statistic::print(std::ostream &os) const {

	static const char *fmt =
"uptime     : %02d:%02d:%02d\n"
"connections: %d\n"
"readed     : %d\n"
"writen     : %d\n"
"read rate  : %d\n"
"write rate : %d\n"
"read ops   : %d\n"
"write ops  : %d\n"
"memory     : %d\n"
"max memory : %d"
;

	os
	<< boost::format(fmt)
		% (seconds/(60*60)) % ((seconds/(60))%60) % (seconds%60)
		% connections
		% format_size(readed)
		% format_size(writen)
		% format_size(read_rate)
		% format_size(write_rate)
		% read_ops
		% write_ops
		% memory
		% max_memory
	;
}

/***************************************************************************/

} // ns yarmi
