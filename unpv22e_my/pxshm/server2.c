#include "cliserv2.h"

int main(int argc, char **argv)
{
	int fd, index, lastnoverflow, temp;
	long offset;
	struct shmstruct *ptr;

	if (argc != 2) {
		fprintf(stderr, "usage: server2 <name>\n");
		exit(1);
	}

	/* 4create shm, set its size, map it, close descriptor */
	shm_unlink(argv[1]);		/* OK if this fails */
	if ((fd = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, FILE_MODE)) == -1) {
		fprintf(stderr, "shm_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if ((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if (ftruncate(fd, sizeof(struct shmstruct)) == -1) {
		perror("ftruncate error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}

	/* 4initialize the array of offsets */
	for (index = 0; index < NMESG; index++)
		ptr->msgoff[index] = index * MESGSIZE;

	/* 4initialize the semaphores in shared memory */
	if (sem_init(&ptr->mutex, 1, 1) == -1) {
		perror("sem_init error");
		exit(1);
	}
	if (sem_init(&ptr->nempty, 1, NMESG) == -1) {
		perror("sem_init error");
		exit(1);
	}
	if (sem_init(&ptr->nstored, 1, 0) == -1) {
		perror("sem_init error");
		exit(1);
	}
	if (sem_init(&ptr->noverflowmutex, 1, 1) == -1) {
		perror("sem_init error");
		exit(1);
	}

		/* 4this program is the consumer */
	index = 0;
	lastnoverflow = 0;
	for ( ; ; ) {
		if (sem_wait(&ptr->nstored) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		if (sem_wait(&ptr->mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		offset = ptr->msgoff[index];
		printf("index = %d: %s\n", index, &ptr->msgdata[offset]);
		if (++index >= NMESG)
			index = 0;				/* circular buffer */
		if (sem_post(&ptr->mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		if (sem_post(&ptr->nempty) == -1) {
			perror("sem_post error");
			exit(1);
		}

		if (sem_wait(&ptr->noverflowmutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		temp = ptr->noverflow;		/* don't printf while mutex held */
		if (sem_post(&ptr->noverflowmutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		if (temp != lastnoverflow) {
			fprintf(stderr, "noverflow = %d\n", temp);
			lastnoverflow = temp;
		}
	}

	exit(0);
}
