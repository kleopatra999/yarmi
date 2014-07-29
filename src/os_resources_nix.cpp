
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

#include <yarmi/throw.hpp>
#include <yarmi/os_resources.hpp>

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <sys/types.h>
#include <unistd.h>

namespace yarmi {
namespace detail {

/***************************************************************************/

struct pstat {
	std::size_t utime_ticks;
	std::size_t cutime_ticks;
	std::size_t stime_ticks;
	std::size_t cstime_ticks;
	std::size_t vm;
	std::size_t rssm;
	std::size_t cpu_total_time;
};

int get_usage(struct pstat* result) {
	memset(result, 0, sizeof(*result));

	enum { num_of_cores = 16 };
	std::vector<std::size_t> cpu_times(num_of_cores, 0ul);

	static const char *self_stat = "/proc/self/stat";
	static const char *stat = "/proc/stat";

	FILE *fstat = fopen(self_stat, "r");
	if ( ! fstat )
		YARMI_THROW("fopen(\"%1%\") error", self_stat);

	std::size_t rss = 0;
	int res = fscanf(fstat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %ld %ld %*d %*d %*d %*d %*u %lu %ld"
		,&result->utime_ticks
		,&result->stime_ticks
		,&result->cutime_ticks
		,&result->cstime_ticks
		,&result->vm
		,&rss
	);
	fclose(fstat);

	if ( res == EOF )
		YARMI_THROW("fscanf() error");

	result->rssm = rss * getpagesize();

	fstat = fopen(stat, "r");
	if ( ! fstat )
		YARMI_THROW("fopen(\"%1%\") error", stat);

	res = fscanf(fstat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu"
		,&cpu_times[0 ]
		,&cpu_times[1 ]
		,&cpu_times[2 ]
		,&cpu_times[3 ]
		,&cpu_times[4 ]
		,&cpu_times[5 ]
		,&cpu_times[6 ]
		,&cpu_times[7 ]
		,&cpu_times[8 ]
		,&cpu_times[9 ]
		,&cpu_times[10]
		,&cpu_times[11]
		,&cpu_times[12]
		,&cpu_times[13]
		,&cpu_times[14]
		,&cpu_times[15]
	);
	fclose(fstat);

	if ( res == EOF )
		YARMI_THROW("fscanf() error");

	result->cpu_total_time = std::accumulate(cpu_times.begin(), cpu_times.end(), 0ul);

	return 0;
}

void calc_cpu_usage(const pstat &cur_usage, const pstat &last_usage, std::size_t *ucpu_usage, std::size_t *scpu_usage, std::size_t *total_cpu_usage) {
	const std::size_t number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
	const std::size_t total_time_diff = cur_usage.cpu_total_time - last_usage.cpu_total_time;

	*ucpu_usage = (((cur_usage.utime_ticks + cur_usage.cutime_ticks)
						 - (last_usage.utime_ticks + last_usage.cutime_ticks))
						 /(double)total_time_diff) * 100 * number_of_processors;
	*scpu_usage = (((cur_usage.stime_ticks + cur_usage.cstime_ticks)
						 - (last_usage.stime_ticks + last_usage.cstime_ticks))
						 /(double)total_time_diff) * 100 * number_of_processors;
	*total_cpu_usage = (*ucpu_usage) + (*scpu_usage);
}

/***************************************************************************/

struct pstat ps1;

void get_resources_usage(
	 std::size_t *virtual_memory_usage
	,std::size_t *resident_memory_usage
	,std::size_t *user_cpu_usage
	,std::size_t *system_cpu_usage
	,std::size_t *total_cpu_usage
) {
	static bool inited = false;
	if ( !inited ) {
		get_usage(&ps1);
		inited = true;
		return;
	}

	struct pstat ps2;
	get_usage(&ps2);

	*virtual_memory_usage = ps2.vm;
	*resident_memory_usage= ps2.rssm;
	calc_cpu_usage(ps2, ps1, user_cpu_usage, system_cpu_usage, total_cpu_usage);

	memcpy(&ps1, &ps2, sizeof(ps1));
}

} // ns detail
} // ns yarmi
