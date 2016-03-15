#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv)
{
	int c, fd, flags;
	char *ptr;
	off_t length;

	flags = O_RDWR | O_CREAT;
	while ((c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
		case 'e':
			flags |= O_EXCL;
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 2) {
		fprintf(stderr, "usage: shmcreate [ -e ] <name> <length>\n");
		exit(1);
	}
	length = atoi(argv[optind + 1]);

	if ((fd = shm_open(argv[optind], flags, FILE_MODE)) == -1) {
		fprintf(stderr, "shm_open error for %s: %s\n", argv[optind], strerror(errno));
		exit(1);
	}
	if (ftruncate(fd, length) == -1) {
		perror("ftruncate error");
		exit(1);
	}

	if ((ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}

	exit(0);
}
