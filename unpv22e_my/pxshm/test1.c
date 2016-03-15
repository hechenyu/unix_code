#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

inline int max(int a, int b)
{
	return a < b ? b : a;
}

int main(int argc, char **argv)
{
	int fd, i;
	char *ptr;
	size_t shmsize, mmapsize, pagesize;

	if (argc != 4) {
		fprintf(stderr, "usage: test1 <name> <shmsize> <mmapsize>\n");
		exit(1);
	}
	shmsize = atoi(argv[2]);
	mmapsize = atoi(argv[3]);

	/* open shm: create or truncate; set shm size */
	if ((fd = shm_open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) == -1) {
		fprintf(stderr, "shm_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (ftruncate(fd, shmsize) == -1) {
		perror("ftruncate error");
		exit(1);
	}
	/* $$.bp$$ */
	if ((ptr = mmap(NULL, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}

	if ((pagesize = sysconf(_SC_PAGESIZE)) == -1) {
		if (errno != 0)
			perror("sysconf error");
		else
			fprintf(stderr, "sysconf %d not defined\n", _SC_PAGESIZE);
		exit(1);
	}
	printf("PAGESIZE = %ld\n", (long) pagesize);

	for (i = 0; i < max(shmsize, mmapsize); i += pagesize) {
		printf("ptr[%d] = %d\n", i, ptr[i]);
		ptr[i] = 1;
		printf("ptr[%d] = %d\n", i + pagesize - 1, ptr[i + pagesize - 1]);
		ptr[i + pagesize - 1] = 1;
	}
	printf("ptr[%d] = %d\n", i, ptr[i]);
	
	exit(0);
}
