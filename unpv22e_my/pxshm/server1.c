#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

struct shmstruct {	/* struct stored in shared memory */
	int count;
};
sem_t *mutex;		/* pointer to named semaphore */

int
main(int argc, char **argv)
{
	int fd;
	struct shmstruct *ptr;

	if (argc != 3) {
		fprintf(stderr, "usage: server1 <shmname> <semname>\n");
		exit(1);
	}

	shm_unlink(argv[1]);		/* OK if this fails */
	/* 4create shm, set its size, map it, close descriptor */
	if ((fd = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, FILE_MODE)) == -1) {
		fprintf(stderr, "shm_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (ftruncate(fd, sizeof(struct shmstruct)) == -1) {
		perror("ftruncate error");
		exit(1);
	}
	if ((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}

	sem_unlink(argv[2]);		/* OK if this fails */
	if ((mutex = sem_open(argv[2], O_CREAT | O_EXCL, FILE_MODE, 1)) == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", argv[2], strerror(errno));
		exit(1);
	}
	if (sem_close(mutex) == -1) {
		perror("sem_close error");
		exit(1);
	}

	exit(0);
}
