#include "cliserv2.h"

int sleep_us(unsigned int nusecs);

int main(int argc, char **argv)
{
	int fd, i, nloop, nusec;
	pid_t pid;
	char mesg[MESGSIZE];
	long offset;
	struct shmstruct *ptr;

	if (argc != 4) {
		fprintf(stderr, "usage: client2 <name> <#loops> <#usec>\n");
		exit(1);
	}
	nloop = atoi(argv[2]);
	nusec = atoi(argv[3]);

	/* 4open and map shared memory that server must create */
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

	pid = getpid();
	for (i = 0; i < nloop; i++) {
		if (sleep_us(nusec) == -1) {
			perror("sleep_us error");
			exit(1);
		}
		snprintf(mesg, MESGSIZE, "pid %ld: message %d", (long) pid, i);

		if (sem_trywait(&ptr->nempty) == -1) {
			if (errno == EAGAIN) {
				if (sem_wait(&ptr->noverflowmutex) == -1) {
					perror("sem_wait error");
					exit(1);
				}
				ptr->noverflow++;
				if (sem_post(&ptr->noverflowmutex) == -1) {
					perror("sem_post error");
					exit(1);
				}
				continue;
			} else {
				perror("sem_trywait error");
				exit(1);
			}
		}
		if (sem_wait(&ptr->mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		offset = ptr->msgoff[ptr->nput];
		if (++(ptr->nput) >= NMESG)
			ptr->nput = 0;		/* circular buffer */
		if (sem_post(&ptr->mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		strcpy(&ptr->msgdata[offset], mesg);
		if (sem_post(&ptr->nstored) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
	exit(0);
}
