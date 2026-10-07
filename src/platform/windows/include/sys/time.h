#ifndef NEXTUFS_WINDOWS_SYS_TIME_H
#define NEXTUFS_WINDOWS_SYS_TIME_H

#include <time.h>

#ifndef _WINSOCKAPI_
struct timeval {
	long tv_sec;
	long tv_usec;
};
#endif

int gettimeofday(struct timeval *time_value, void *timezone);

#endif
