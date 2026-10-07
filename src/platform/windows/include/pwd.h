#ifndef NEXTUFS_WINDOWS_PWD_H
#define NEXTUFS_WINDOWS_PWD_H

struct passwd {
	char *pw_name;
};

struct passwd *getpwuid(int uid);

#endif
