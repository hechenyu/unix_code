#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

char *
gf_time(void);

void *child_thread(void *arg);

int main()
{
	int err;
	pthread_t tid;

	if ((err = pthread_rwlock_rdlock(&rwlock)) != 0) {	/* parent read locks entire file */
		fprintf(stderr, "pthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: parent has read lock\n", gf_time());

	if ((err = pthread_create(&tid, NULL, child_thread, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	sleep(4);

	if ((err = pthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "pthread_rwlock_unlock error: %s\n", strerror(err));
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
	if ((err = pthread_rwlock_rdlock(&rwlock)) != 0) {	/* this should work */
		fprintf(stderr, "pthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: child has read lock\n", gf_time());
	sleep(2);
	if ((err = pthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "pthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: child releases read lock\n", gf_time());

	if ((err = pthread_rwlock_rdlock(&rwlock)) != 0) {	/* this should work */
		fprintf(stderr, "pthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: child has read lock\n", gf_time());
	sleep(2);
	return NULL;
}
