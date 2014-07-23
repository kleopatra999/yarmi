
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

#include <yarmi/os_resources.hpp>

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

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

/*
 * read /proc data into the passed struct pstat
 * returns 0 on success, -1 on error
 */
int get_usage(struct pstat* result) {
	bzero(result, sizeof(struct pstat));

	FILE *fpstat = fopen("/proc/self/stat", "r");
	if (fpstat == NULL) {
		perror("FOPEN ERROR ");
		return -1;
	}

	std::size_t rss;
	if (fscanf(fpstat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu"
				  "%lu %ld %ld %*d %*d %*d %*d %*u %lu %ld",
				  &result->utime_ticks, &result->stime_ticks,
				  &result->cutime_ticks, &result->cstime_ticks, &result->vm,
				  &rss) == EOF) {
		fclose(fpstat);
		return -1;
	}
	fclose(fpstat);
	result->rssm = rss * getpagesize();

	FILE *fstat = fopen("/proc/stat", "r");
	if (fstat == NULL) {
		perror("FOPEN ERROR ");
		fclose(fstat);
		return -1;
	}

	//read+calc cpu total time from /proc/stat
	std::size_t cpu_time[10];
	bzero(cpu_time, sizeof(cpu_time));
	if (fscanf(fstat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
				  &cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
				  &cpu_time[4], &cpu_time[5], &cpu_time[6], &cpu_time[7],
				  &cpu_time[8], &cpu_time[9]) == EOF) {
		fclose(fstat);
		return -1;
	}

	fclose(fstat);

	for(int i=0; i < 10;i++)
		result->cpu_total_time += cpu_time[i];

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
