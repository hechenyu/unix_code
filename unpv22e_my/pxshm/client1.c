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

int main(int argc, char **argv)
{
	int fd, i, nloop;
	pid_t pid;
	struct shmstruct *ptr;

	if (argc != 4) {
		fprintf(stderr, "usage: client1 <shmname> <semname> <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[3]);

	if ((fd = shm_open(argv[1], O_RDWR, FILE_MODE)) == -1) {
		fprintf(stderr, "shm_open error for %s: %s\n", argv[1], strerror(errno));
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

	if ((mutex = sem_open(argv[2], 0)) == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", argv[2], strerror(errno));
		exit(1);
	}

	pid = getpid();
	for (i = 0; i < nloop; i++) {
		if (sem_wait(mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		printf("pid %ld: %d\n", (long) pid, ptr->count++);
		if (sem_post(mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
	exit(0);
}
