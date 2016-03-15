#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: shmunlink <name>\n");
		exit(1);
	}

	if (shm_unlink(argv[1]) == -1) {
		perror("shm_unlink error");
		exit(1);
	}

	exit(0);
}
