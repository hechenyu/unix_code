#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int main(int argc, char **argv)
{
	int fd[2];
	char buff[7];
	struct stat info;

	if (argc != 2) {
		fprintf(stderr, "usage: test1 <pathname>\n");
		exit(1);
	}

	if ((mkfifo(argv[1], FILE_MODE) < 0) && (errno != EEXIST)) {
		fprintf(stderr, "can't create %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if ((fd[0] = open(argv[1], O_RDONLY | O_NONBLOCK)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if ((fd[1] = open(argv[1], O_WRONLY | O_NONBLOCK)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	/* 4check sizes when FIFO is empty */
	if (fstat(fd[0], &info) == -1) {
		perror("fstat error");
		exit(1);
	}
	printf("fd[0]: st_size = %ld\n", (long) info.st_size);
	if (fstat(fd[1], &info) == -1) {
		perror("fstat error");
		exit(1);
	}
	printf("fd[1]: st_size = %ld\n", (long) info.st_size);

	if (write(fd[1], buff, sizeof(buff)) != sizeof(buff)) {
		perror("write error");
		exit(1);
	}

	/* 4check sizes when FIFO contains 7 bytes */
	if (fstat(fd[0], &info) == -1) {
		perror("fstat error");
		exit(1);
	}
	printf("fd[0]: st_size = %ld\n", (long) info.st_size);
	if (fstat(fd[1], &info) == -1) {
		perror("fstat error");
		exit(1);
	}
	printf("fd[1]: st_size = %ld\n", (long) info.st_size);

	exit(0);
}
