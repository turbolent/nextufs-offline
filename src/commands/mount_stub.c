#include <stdio.h>

int
nextufs_mount_main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	fprintf(stderr, "nextufs: mount support is unavailable in this macOS build\n");
	return 2;
}

int nextufs_fsck_main(int argc, char **argv) { return nextufs_mount_main(argc, argv); }
int nextufs_mkimg_main(int argc, char **argv) { return nextufs_mount_main(argc, argv); }
int nextufs_resize_main(int argc, char **argv) { return nextufs_mount_main(argc, argv); }
