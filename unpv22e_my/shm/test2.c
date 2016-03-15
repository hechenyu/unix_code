#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	FILE	"test.data"
#define	SIZE	32768

int
main(int argc, char **argv)
{
	int		fd, i;
	char	*ptr;

		/* 4open: create or truncate; then mmap file */
	if ((fd = open(FILE, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", FILE, strerror(errno));
		exit(1);
	}
	if ((ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}

	for (i = 4096; i <= SIZE; i += 4096) {
		printf("setting file size to %d\n", i);
		if (ftruncate(fd, i) == -1) {
			perror("ftruncate error");
			exit(1);
		}
		printf("ptr[%d] = %d\n", i-1, ptr[i-1]);
	}
	
	exit(0);
}
