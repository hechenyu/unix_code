#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

char *
gf_time(void);

void *thread1(void *arg);

void *thread2(void *arg);

int main()
{
	pthread_t tid1, tid2;
	int err;

	if ((err = pthread_rwlock_rdlock(&rwlock)) != 0) {	/* parent read locks entire file */
		fprintf(stderr, "pthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: parent has read lock\n", gf_time());

	if ((err = pthread_create(&tid1, NULL, thread1, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	if ((err = pthread_create(&tid2, NULL, thread2, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	/* 4parent */
	sleep(5);
	if ((err = pthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "pthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: parent releases read lock\n", gf_time());

	if ((err = pthread_join(tid1, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}

	if ((err = pthread_join(tid2, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}

	exit(0);
}

void *thread1(void *arg)
{
	int err;
	sleep(1);
	printf("%s: first child tries to obtain write lock\n", gf_time());
	if ((err = pthread_rwlock_wrlock(&rwlock)) != 0) {	/* this should block */
		fprintf(stderr, "pthread_rwlock_wrlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: first child obtains write lock\n", gf_time());
	sleep(2);
	if ((err = pthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "pthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: first child releases write lock\n", gf_time());
	return NULL;
}

void *thread2(void *arg)
{
	int err;
		/* 4second child */
	sleep(3);
	printf("%s: second child tries to obtain read lock\n", gf_time());
	if ((err = pthread_rwlock_rdlock(&rwlock)) != 0) {
		fprintf(stderr, "pthread_rwlock_rdlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: second child obtains read lock\n", gf_time());
	sleep(4);
	if ((err = pthread_rwlock_unlock(&rwlock)) != 0) {
		fprintf(stderr, "pthread_rwlock_unlock error: %s\n", strerror(err));
		exit(1);
	}
	printf("%s: second child releases read lock\n", gf_time());
	return NULL;
}
