#include <pthread.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pthread_rwlock.h"

#define	MAXNTHREADS	100

void	*reader(void *), *writer(void *);

int		nloop = 1000, nreaders = 6, nwriters = 4;

struct {
  mypthread_rwlock_t	rwlock;
  pthread_mutex_t	rcountlock;
  int				nreaders;
  int				nwriters;
} shared = { MYPTHREAD_RWLOCK_INITIALIZER, PTHREAD_MUTEX_INITIALIZER };

int
main(int argc, char **argv)
{
	int		c, i;
	pthread_t	tid_readers[MAXNTHREADS], tid_writers[MAXNTHREADS];
	int err;

	while ((c = getopt(argc, argv, "n:r:w:")) != -1) {
		switch (c) {
		case 'n':
			nloop = atoi(optarg);
			break;

		case 'r':
			nreaders = atoi(optarg);
			break;

		case 'w':
			nwriters = atoi(optarg);
			break;

		case '?':
			exit(1);
		}
	}
	if (optind != argc) {
		fprintf(stderr, "usage: test1 [-n #loops ] [ -r #readers ] [ -w #writers ]\n");
		exit(1);
	}

		/* 4create all the reader and writer threads */
	if ((err = pthread_setconcurrency(nreaders + nwriters)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}
	for (i = 0; i < nreaders; i++) {
		if ((err = pthread_create(&tid_readers[i], NULL, reader, NULL)) != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}
	for (i = 0; i < nwriters; i++) {
		if ((err = pthread_create(&tid_writers[i], NULL, writer, NULL)) != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}

		/* wait for all the threads to complete */
	for (i = 0; i < nreaders; i++) {
		if ((err = pthread_join(tid_readers[i], NULL)) != 0) {
			fprintf(stderr, "pthread_join error: %s\n", strerror(err));
			exit(1);
		}
	}
	for (i = 0; i < nwriters; i++) {
		if ((err = pthread_join(tid_writers[i], NULL)) != 0) {
			fprintf(stderr, "pthread_join error: %s\n", strerror(err));
			exit(1);
		}
	}

	exit(0);
}

void *
reader(void *arg)
{
	int		i;
	int err;

	for (i = 0; i < nloop; i++) {
		if ((err = mypthread_rwlock_rdlock(&shared.rwlock)) != 0) {
			fprintf(stderr, "mypthread_rwlock_rdlock error: %s\n", strerror(err));
			exit(1);
		}

		if ((err = pthread_mutex_lock(&shared.rcountlock)) != 0) {
			fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
			exit(1);
		}
		shared.nreaders++;	/* shared by all readers; must protect */
		if ((err = pthread_mutex_unlock(&shared.rcountlock)) != 0) {
			fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
			exit(1);
		}

		if (shared.nwriters > 0) {
			fprintf(stderr, "reader: %d writers found\n", shared.nwriters);
			exit(1);
		}

		if ((err = pthread_mutex_lock(&shared.rcountlock)) != 0) {
			fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
			exit(1);
		}
		shared.nreaders--;	/* shared by all readers; must protect */
		if ((err = pthread_mutex_unlock(&shared.rcountlock)) != 0) {
			fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
			exit(1);
		}

		if ((err = mypthread_rwlock_unlock(&shared.rwlock)) != 0) {
			fprintf(stderr, "mypthread_rwlock_unlock error: %s\n", strerror(err));
			exit(1);
		}
	}
	return(NULL);
}

void *
writer(void *arg)
{
	int		i;
	int err;

	for (i = 0; i < nloop; i++) {
		if ((err = mypthread_rwlock_wrlock(&shared.rwlock)) != 0) {
			fprintf(stderr, "mypthread_rwlock_wrlock error: %s\n", strerror(err));
			exit(1);
		}

		shared.nwriters++;	/* only one writer; need not protect */

		if (shared.nwriters > 1) {
			fprintf(stderr, "writer: %d writers found\n", shared.nwriters);
			exit(1);
		}
		if (shared.nreaders > 0) {
			fprintf(stderr, "writer: %d readers found\n", shared.nreaders);
			exit(1);
		}

		shared.nwriters--;	/* only one writer; need not protect */
		if ((err = mypthread_rwlock_unlock(&shared.rwlock)) != 0) {
			fprintf(stderr, "mypthread_rwlock_unlock error: %s\n", strerror(err));
			exit(1);
		}
	}
	return(NULL);
}
