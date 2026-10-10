#include "nextufs_windows_compat.h"

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <dirent.h>
#include <libgen.h>
#include <pwd.h>
#include <sys/time.h>
#include <unistd.h>

int
nextufs_windows_open(const char *path, int flags, ...)
{
	int mode;
	va_list args;

	flags |= _O_BINARY;
	if ((flags & _O_CREAT) == 0)
		return _open(path, flags);
	va_start(args, flags);
	mode = va_arg(args, int);
	va_end(args);
	return _open(path, flags, mode);
}

int
ftruncate(int fd, off_t length)
{
	errno_t error = _chsize_s(fd, length);

	if (error != 0) {
		errno = (int)error;
		return -1;
	}
	return 0;
}

int
fsync(int fd)
{
	return _commit(fd);
}

static ssize_t
nextufs_windows_positioned_io(int fd, void *buffer, size_t size, off_t offset,
	int writing)
{
	__int64 original;
	int count;
	int saved_errno;

	original = _lseeki64(fd, 0, SEEK_CUR);
	if (original < 0)
		return -1;
	if (_lseeki64(fd, offset, SEEK_SET) < 0)
		return -1;
	if (size > INT_MAX)
		size = INT_MAX;
	count = writing ? _write(fd, buffer, (unsigned int)size) :
	    _read(fd, buffer, (unsigned int)size);
	saved_errno = errno;
	if (_lseeki64(fd, original, SEEK_SET) < 0 && count >= 0)
		return -1;
	errno = saved_errno;
	return count;
}

static int
nextufs_windows_errno(DWORD error)
{
	switch (error) {
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
	case ERROR_INVALID_NAME:
		return ENOENT;
	case ERROR_ACCESS_DENIED:
		return EACCES;
	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
		return ENOMEM;
	default:
		return EIO;
	}
}

ssize_t
pread(int fd, void *buffer, size_t size, off_t offset)
{
	return nextufs_windows_positioned_io(fd, buffer, size, offset, 0);
}

ssize_t
pwrite(int fd, const void *buffer, size_t size, off_t offset)
{
	return nextufs_windows_positioned_io(fd, (void *)buffer, size, offset, 1);
}

char *
realpath(const char *path, char *resolved)
{
	return _fullpath(resolved, path, PATH_MAX);
}

DIR *
opendir(const char *path)
{
	DIR *directory;
	char pattern[MAX_PATH];
	size_t path_length;
	int written;

	directory = (DIR *)calloc(1, sizeof(*directory));
	if (directory == NULL)
		return NULL;
	path_length = strlen(path);
	written = snprintf(pattern, sizeof(pattern), "%s%s*",
	    path, path_length != 0 &&
	    path[path_length - 1] != '\\' && path[path_length - 1] != '/' ?
	    "\\" : "");
	if (written < 0 || (size_t)written >= sizeof(pattern)) {
		free(directory);
		errno = ENAMETOOLONG;
		return NULL;
	}
	directory->handle = FindFirstFileA(pattern, &directory->find_data);
	if (directory->handle == INVALID_HANDLE_VALUE) {
		DWORD error = GetLastError();
		free(directory);
		errno = nextufs_windows_errno(error);
		return NULL;
	}
	directory->first = 1;
	return directory;
}

struct dirent *
readdir(DIR *directory)
{
	BOOL found;

	if (directory->first) {
		directory->first = 0;
		found = TRUE;
	} else {
		found = FindNextFileA(directory->handle, &directory->find_data);
	}
	if (!found) {
		DWORD error = GetLastError();
		if (error != ERROR_NO_MORE_FILES)
			errno = nextufs_windows_errno(error);
		return NULL;
	}
	strncpy_s(directory->entry.d_name, sizeof(directory->entry.d_name),
	    directory->find_data.cFileName, _TRUNCATE);
	return &directory->entry;
}

int
closedir(DIR *directory)
{
	BOOL closed;

	closed = FindClose(directory->handle);
	free(directory);
	if (!closed) {
		errno = EIO;
		return -1;
	}
	return 0;
}

char *
basename(char *path)
{
	char *base = path;
	char *cursor;

	for (cursor = path; *cursor != '\0'; cursor++)
		if (*cursor == '/' || *cursor == '\\')
			base = cursor + 1;
	return base;
}

char *
strsep(char **stringp, const char *delimiters)
{
	char *start;
	char *end;

	if (stringp == NULL || *stringp == NULL)
		return NULL;
	start = *stringp;
	end = strpbrk(start, delimiters);
	if (end == NULL) {
		*stringp = NULL;
		return start;
	}
	*end = '\0';
	*stringp = end + 1;
	return start;
}

char *
rindex(char *string, int character)
{
	return strrchr(string, character);
}

void
bcopy(const void *source, void *destination, size_t size)
{
	memmove(destination, source, size);
}

int
bcmp(const void *left, const void *right, size_t size)
{
	return memcmp(left, right, size);
}

unsigned int
sleep(unsigned int seconds)
{
	Sleep(seconds * 1000U);
	return 0;
}

struct passwd *
getpwuid(int uid)
{
	(void)uid;
	return NULL;
}

int
gettimeofday(struct timeval *time_value, void *timezone)
{
	FILETIME file_time;
	ULARGE_INTEGER ticks;

	(void)timezone;
	if (time_value == NULL) {
		errno = EINVAL;
		return -1;
	}
	GetSystemTimeAsFileTime(&file_time);
	ticks.LowPart = file_time.dwLowDateTime;
	ticks.HighPart = file_time.dwHighDateTime;
	ticks.QuadPart -= 116444736000000000ULL;
	time_value->tv_sec = (long)(ticks.QuadPart / 10000000ULL);
	time_value->tv_usec = (long)((ticks.QuadPart % 10000000ULL) / 10ULL);
	return 0;
}
