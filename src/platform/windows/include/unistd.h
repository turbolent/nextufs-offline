#ifndef NEXTUFS_WINDOWS_UNISTD_H
#define NEXTUFS_WINDOWS_UNISTD_H

#include <stddef.h>
#include <sys/types.h>

int nextufs_windows_open(const char *path, int flags, ...);
int ftruncate(int fd, off_t length);
int fsync(int fd);
ssize_t pread(int fd, void *buffer, size_t size, off_t offset);
ssize_t pwrite(int fd, const void *buffer, size_t size, off_t offset);
char *realpath(const char *path, char *resolved);

#define open(...) nextufs_windows_open(__VA_ARGS__)
#define lseek(fd, offset, origin) _lseeki64((fd), (offset), (origin))
#define rmdir(path) _rmdir(path)

#endif
