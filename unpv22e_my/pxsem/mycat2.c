/* include main */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define	BUFFSIZE	8192
#define	NBUFF	 8

struct {	/* data shared by producer and consumer */
	struct {
		char data[BUFFSIZE];	/* a buffer */
		ssize_t	n;		/* count of #bytes in the buffer */
	} buff[NBUFF];		/* NBUFF of these buffers/counts */
	sem_t mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

int fd;		/* input file to copy to stdout */
void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
	pthread_t tid_produce, tid_consume;
	int err;

	if (argc != 2) {
		fprintf(stderr, "usage: mycat2 <pathname>\n");
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	/* 4initialize three semaphores */
	if (sem_init(&shared.mutex, 0, 1) == -1) {
		perror("sem_init error");
		exit(1);
	}
	if (sem_init(&shared.nempty, 0, NBUFF) == -1) {
		perror("sem_init error");
		exit(1);
	}
	if (sem_init(&shared.nstored, 0, 0) == -1) {
		perror("sem_init error");
		exit(1);
	}

	/* 4one producer thread, one consumer thread */
#ifdef __USE_UNIX98
	if ((err = pthread_setconcurrency(2)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}
#endif
	if ((err = pthread_create(&tid_produce, NULL, produce, NULL)) != 0) {	/* reader thread */
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_create(&tid_consume, NULL, consume, NULL)) != 0) {	/* writer thread */
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	if ((err = pthread_join(tid_produce, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_join(tid_consume, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}

	if (sem_destroy(&shared.mutex) == -1) {
		perror("sem_destroy error");
		exit(1);
	}
	if (sem_destroy(&shared.nempty) == -1) {
		perror("sem_destroy error");
		exit(1);
	}
	if (sem_destroy(&shared.nstored) == -1) {
		perror("sem_destroy error");
		exit(1);
	}
	exit(0);
}
/* end main */

/* include prodcons */
void *produce(void *arg)
{
	int i;

	for (i = 0; ; ) {
		/* wait for at least 1 empty slot */
		if (sem_wait(&shared.nempty) == -1) {
			perror("sem_wait error");
			exit(1);
		}

		if (sem_wait(&shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		/* 4critical region */
		if (sem_post(&shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}

		shared.buff[i].n = read(fd, shared.buff[i].data, BUFFSIZE);
		if (shared.buff[i].n == -1) {
			perror("read error");
			exit(1);
		}
		if (shared.buff[i].n == 0) {
			if (sem_post(&shared.nstored) == -1) {	/* 1 more stored item */
				perror("sem_post error");
				exit(1);
			}
			return(NULL);
		}
		if (++i >= NBUFF)
			i = 0;					/* circular buffer */

		/* 1 more stored item */
		if (sem_post(&shared.nstored) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
}

void *consume(void *arg)
{
	int i;

	for (i = 0; ; ) {
		/* wait for at least 1 stored item */
		if (sem_wait(&shared.nstored) == -1) {
			perror("sem_wait error");
			exit(1);
		}

		if (sem_wait(&shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		/* 4critical region */
		if (sem_post(&shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}

		if (shared.buff[i].n == 0)
			return(NULL);
		if (write(STDOUT_FILENO, shared.buff[i].data, shared.buff[i].n) != shared.buff[i].n) {
			perror("write error");
			exit(1);
		}
		if (++i >= NBUFF)
			i = 0;					/* circular buffer */

		/* 1 more empty slot */
		if (sem_post(&shared.nempty) == -1 ) {
			perror("sem_post error");
			exit(1);
		}
	}
}
/* end prodcons */

