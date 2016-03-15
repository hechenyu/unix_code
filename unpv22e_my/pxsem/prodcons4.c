/* include globals */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define	NBUFF		10
#define	MAXNTHREADS	100

int nitems, nproducers, nconsumers;		/* read-only */

struct {	/* data shared by producers and consumers */
	int buff[NBUFF];
	int nput;		/* item number: 0, 1, 2, ... */
	int nputval;		/* value to store in buff[] */
	int nget;		/* item number: 0, 1, 2, ... */
	int ngetval;		/* value fetched from buff[] */
	sem_t mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}
void *produce(void *), *consume(void *);
/* end globals */

/* include main */
int main(int argc, char **argv)
{
	int i, prodcount[MAXNTHREADS], conscount[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume[MAXNTHREADS];
	int err;

	if (argc != 4) {
		fprintf(stderr, "usage: prodcons4 <#items> <#producers> <#consumers>\n");
		exit(1);
	}
	nitems = atoi(argv[1]);
	nproducers = min(atoi(argv[2]), MAXNTHREADS);
	nconsumers = min(atoi(argv[3]), MAXNTHREADS);

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

	/* 4create all producers and all consumers */
#ifdef __USE_UNIX98
	if ((err = pthread_setconcurrency(nproducers + nconsumers)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}
#endif
	for (i = 0; i < nproducers; i++) {
		prodcount[i] = 0;
		if ((err = pthread_create(&tid_produce[i], NULL, produce, &prodcount[i])) != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}
	for (i = 0; i < nconsumers; i++) {
		conscount[i] = 0;
		if ((err = pthread_create(&tid_consume[i], NULL, consume, &conscount[i])) != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}

	/* 4wait for all producers and all consumers */
	for (i = 0; i < nproducers; i++) {
		if ((err = pthread_join(tid_produce[i], NULL)) != 0) {
			fprintf(stderr, "pthread_join error: %s\n", strerror(err));
			exit(1);
		}
		printf("producer count[%d] = %d\n", i, prodcount[i]);	
	}
	for (i = 0; i < nconsumers; i++) {
		if ((err = pthread_join(tid_consume[i], NULL)) != 0) {
			fprintf(stderr, "pthread_join error: %s\n", strerror(err));
			exit(1);
		}
		printf("consumer count[%d] = %d\n", i, conscount[i]);	
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

/* include produce */
void *produce(void *arg)
{
	for ( ; ; ) {
		/* wait for at least 1 empty slot */
		if (sem_wait(&shared.nempty) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		if (sem_wait(&shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}

		if (shared.nput >= nitems) {
			/* let consumers terminate */
			if (sem_post(&shared.nstored) == -1) {
				perror("sem_post error");
				exit(1);
			}
			if (sem_post(&shared.nempty) == -1 ) {
				perror("sem_post error");
				exit(1);
			}
			if (sem_post(&shared.mutex) == -1) {
				perror("sem_post error");
				exit(1);
			}
			return(NULL);			/* all done */
		}

		shared.buff[shared.nput % NBUFF] = shared.nputval;
		shared.nput++;
		shared.nputval++;

		if (sem_post(&shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		/* 1 more stored item */
		if (sem_post(&shared.nstored) == -1) {
			perror("sem_post error");
			exit(1);
		}
		*((int *) arg) += 1;
	}
}
/* end produce */

/* include consume */
void *consume(void *arg)
{
	int i;

	for ( ; ; ) {
		/* wait for at least 1 stored item */
		if (sem_wait(&shared.nstored) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		if (sem_wait(&shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}

		if (shared.nget >= nitems) {
			if (sem_post(&shared.nstored) == -1) {
				perror("sem_post error");
				exit(1);
			}
			if (sem_post(&shared.mutex) == -1) {
				perror("sem_post error");
				exit(1);
			}
			return(NULL);			/* all done */
		}

		i = shared.nget % NBUFF;
		if (shared.buff[i] != shared.ngetval)
			printf("error: buff[%d] = %d\n", i, shared.buff[i]);
		shared.nget++;
		shared.ngetval++;

		if (sem_post(&shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		/* 1 more empty slot */
		if (sem_post(&shared.nempty) == -1 ) {
			perror("sem_post error");
			exit(1);
		}
		*((int *) arg) += 1;
	}
}
/* end consume */
