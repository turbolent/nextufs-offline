#ifndef NEXTUFS_WINDOWS_DIRENT_H
#define NEXTUFS_WINDOWS_DIRENT_H

#include <windows.h>

struct dirent {
	char d_name[MAX_PATH];
};

typedef struct {
	HANDLE handle;
	WIN32_FIND_DATAA find_data;
	struct dirent entry;
	int first;
} DIR;

DIR *opendir(const char *path);
struct dirent *readdir(DIR *directory);
int closedir(DIR *directory);

#endif
