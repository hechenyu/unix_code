#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv)
{
	int i, fd;
	struct stat stat;
	unsigned char c, *ptr;

	if (argc != 2) {
		fprintf(stderr, "usage: shmread <name>\n");
		exit(1);
	}

	/* 4open, get size, map */
	if ((fd = shm_open(argv[1], O_RDONLY, FILE_MODE)) == -1) {
		fprintf(stderr, "shm_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (fstat(fd, &stat) == -1) {
		perror("fstat error");
		exit(1);
	}
	if ((ptr = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}

	/* 4check that ptr[0] = 0, ptr[1] = 1, etc. */
	for (i = 0; i < stat.st_size; i++)
		if ((c = *ptr++) != (i % 256)) {
			fprintf(stderr, "ptr[%d] = %d\n", i, c);
			exit(1);
		}

	exit(0);
}
