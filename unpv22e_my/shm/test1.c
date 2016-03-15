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
#define max(a, b) ((a) < (b) ? (a) : (b))

int
main(int argc, char **argv)
{
	int		fd, i;
	char	*ptr;
	size_t	filesize, mmapsize, pagesize;

	if (argc != 4) {
		fprintf(stderr, "usage: test1 <pathname> <filesize> <mmapsize>\n");
		exit(1);
	}
	filesize = atoi(argv[2]);
	mmapsize = atoi(argv[3]);

		/* 4open file: create or truncate; set file size */
	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (lseek(fd, filesize-1, SEEK_SET) == (off_t) -1) {
		perror("lseek error");
		exit(1);
	}
	if (write(fd, "", 1) != 1) {
		perror("write error");
		exit(1);
	}

	if ((ptr = mmap(NULL, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}

	if ((pagesize = sysconf(_SC_PAGESIZE)) == -1) {
		if (errno != 0) {
			perror("sysconf error");
			exit(1);
		} else {
			fprintf(stderr, "sysconf: %d not defined\n", _SC_PAGESIZE);
			exit(1);
		}
	}
	printf("PAGESIZE = %ld\n", (long) pagesize);

	for (i = 0; i < max(filesize, mmapsize); i += pagesize) {
		printf("ptr[%d] = %d\n", i, ptr[i]);
		ptr[i] = 1;
		printf("ptr[%d] = %d\n", i + pagesize - 1, ptr[i + pagesize - 1]);
		ptr[i + pagesize - 1] = 1;
	}
	printf("ptr[%d] = %d\n", i, ptr[i]);
	
	exit(0);
}
