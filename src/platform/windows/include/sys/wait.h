#ifndef NEXTUFS_WINDOWS_SYS_WAIT_H
#define NEXTUFS_WINDOWS_SYS_WAIT_H

#define WIFEXITED(status) (((status) & 0xff) == 0)
#define WEXITSTATUS(status) (((status) >> 8) & 0xff)

#endif
