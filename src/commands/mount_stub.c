#include <stdio.h>

int
nextufs_mount_main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	fprintf(stderr, "nextufs: mount support is unavailable in this offline build\n");
	return 2;
}

#ifndef __linux__
int
nextufs_fsck_main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	fprintf(stderr, "nextufs: fsck support is unavailable in this build\n");
	return 2;
}
#endif
