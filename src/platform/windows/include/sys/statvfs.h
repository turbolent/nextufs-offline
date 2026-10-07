#ifndef NEXTUFS_WINDOWS_SYS_STATVFS_H
#define NEXTUFS_WINDOWS_SYS_STATVFS_H

#include <stdint.h>

struct statvfs {
	uint64_t f_bsize;
	uint64_t f_frsize;
	uint64_t f_blocks;
	uint64_t f_bfree;
	uint64_t f_bavail;
	uint64_t f_files;
	uint64_t f_ffree;
	uint64_t f_favail;
	uint64_t f_fsid;
	uint64_t f_flag;
	uint64_t f_namemax;
};

#endif
