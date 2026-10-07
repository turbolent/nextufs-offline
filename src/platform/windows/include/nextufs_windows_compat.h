#ifndef NEXTUFS_WINDOWS_COMPAT_H
#define NEXTUFS_WINDOWS_COMPAT_H

#include <sys/types.h>
#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

#define stat _stat64
#define fstat _fstat64

typedef unsigned short mode_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef intptr_t ssize_t;
typedef unsigned char u_char;

#define off_t __int64

#ifndef S_IFMT
#define S_IFMT _S_IFMT
#endif
#ifndef S_IFDIR
#define S_IFDIR _S_IFDIR
#endif
#ifndef S_IFREG
#define S_IFREG _S_IFREG
#endif
#ifndef S_IFCHR
#define S_IFCHR _S_IFCHR
#endif
#ifndef S_IFIFO
#define S_IFIFO _S_IFIFO
#endif
#ifndef S_IFBLK
#define S_IFBLK 0060000
#endif
#ifndef S_IFLNK
#define S_IFLNK 0120000
#endif
#ifndef S_IFSOCK
#define S_IFSOCK 0140000
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & _S_IFMT) == _S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(mode) (((mode) & _S_IFMT) == _S_IFREG)
#endif
#ifndef S_ISCHR
#define S_ISCHR(mode) (((mode) & _S_IFMT) == _S_IFCHR)
#endif
#ifndef S_ISBLK
#define S_ISBLK(mode) (((mode) & _S_IFMT) == _S_IFBLK)
#endif
#ifndef S_ISFIFO
#define S_ISFIFO(mode) (((mode) & _S_IFMT) == _S_IFIFO)
#endif
#ifndef S_ISLNK
#define S_ISLNK(mode) (((mode) & _S_IFMT) == _S_IFLNK)
#endif

#define bzero(ptr, size) memset((ptr), 0, (size))
#define strdup _strdup
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtok_r strtok_s
#define fileno _fileno
char *strsep(char **stringp, const char *delimiters);
char *rindex(char *string, int character);
void bcopy(const void *source, void *destination, size_t size);
int bcmp(const void *left, const void *right, size_t size);
unsigned int sleep(unsigned int seconds);

#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_RDWR _O_RDWR
#define O_APPEND _O_APPEND
#define O_CREAT _O_CREAT
#define O_TRUNC _O_TRUNC
#define O_EXCL _O_EXCL
#endif
#ifndef O_BINARY
#define O_BINARY _O_BINARY
#endif
#ifndef R_OK
#define R_OK 4
#define W_OK 2
#define X_OK 1
#define F_OK 0
#endif

#ifndef PATH_MAX
#define PATH_MAX 32768
#endif
#ifndef SIGQUIT
#define SIGQUIT SIGTERM
#endif

#endif
