#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pthread_rwlock.h"

mypthread_rwlock_t rwlock = MYPTHREAD_RWLOCK_INITIALIZER;

char *
gf_time(void);

void *child_thread(void *arg);

int main()
{
	int err;
	pthread_t tid;

	if ((err = mypthread_rwlock_rdlock(&rwlock)) != 0) {	/* parent read locks entire file */
		fprintf(stderr, "mypthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: parent has read lock\n", gf_time());

	if ((err = pthread_create(&tid, NULL, child_thread, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	sleep(4);

	if ((err = mypthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "mypthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: parent releases read lock\n", gf_time());

	if ((err = pthread_join(tid, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}
	exit(0);
}

void *child_thread(void *arg)
{
		/* 4child */
	int err;
	if ((err = mypthread_rwlock_rdlock(&rwlock)) != 0) {	/* this should work */
		fprintf(stderr, "mypthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: child has read lock\n", gf_time());
	sleep(2);
	if ((err = mypthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "mypthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: child releases read lock\n", gf_time());

	if ((err = mypthread_rwlock_rdlock(&rwlock)) != 0) {	/* this should work */
		fprintf(stderr, "mypthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: child has read lock\n", gf_time());
	sleep(2);
	return NULL;
}
