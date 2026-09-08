#include <stdio.h>

int
nextufs_mount_main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	fprintf(stderr, "nextufs: mount support is unavailable in this macOS build\n");
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
int nextufs_mkimg_main(int argc, char **argv) { return nextufs_mount_main(argc, argv); }
int nextufs_resize_main(int argc, char **argv) { return nextufs_mount_main(argc, argv); }
