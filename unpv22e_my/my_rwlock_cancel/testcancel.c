#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pthread_rwlock.h"

mypthread_rwlock_t	rwlock = MYPTHREAD_RWLOCK_INITIALIZER;

void	 *thread1(void *), *thread2(void *);
pthread_t	tid1, tid2;

int
main(int argc, char **argv)
{
	void	*status;
	int err;
	if ((err = mypthread_rwlock_init(&rwlock, NULL)) != 0) {
		fprintf(stderr, "mypthread_rwlock_init error: %s\n", strerror(err));
		exit(1);
	}

	if ((err = pthread_setconcurrency(2)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_create(&tid1, NULL, thread1, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}
	sleep(1);		/* let thread1() get the lock */
	if ((err = pthread_create(&tid2, NULL, thread2, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	if ((err = pthread_join(tid2, &status)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}
	if (status != PTHREAD_CANCELED)
		printf("thread2 status = %p\n", status);
	if ((err = pthread_join(tid1, &status)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}
	if (status != NULL)
		printf("thread1 status = %p\n", status);

	printf("rw_refcount = %d, rw_nwaitreaders = %d, rw_nwaitwriters = %d\n",
		   rwlock.rw_refcount, rwlock.rw_nwaitreaders,
		   rwlock.rw_nwaitwriters);
	if ((err = mypthread_rwlock_destroy(&rwlock)) != 0) {
		fprintf(stderr, "mypthread_rwlock_destroy error: %s\n", strerror(err));
		exit(1);
	}
		/* 4returns EBUSY error if cancelled thread does not cleanup */

	exit(0);
}

void *
thread1(void *arg)
{
	int err;
	if ((err = mypthread_rwlock_rdlock(&rwlock)) != 0) {
		fprintf(stderr, "mypthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("thread1() got a read lock\n");
	sleep(3);		/* let thread2 block in mypthread_rwlock_wrlock() */
	if ((err = pthread_cancel(tid2)) != 0) {
		fprintf(stderr, "pthread_cancel error: %s\n", strerror(err));
		exit(1);
	}
	sleep(3);
	if ((err = mypthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "mypthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	return(NULL);
}

void *
thread2(void *arg)
{
	int err;
	printf("thread2() trying to obtain a write lock\n");
	if ((err = mypthread_rwlock_wrlock(&rwlock)) != 0) {
		fprintf(stderr, "mypthread_rwlock_wrlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("thread2() got a write lock\n");
	sleep(1);
	if ((err = mypthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "mypthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	return(NULL);
}
