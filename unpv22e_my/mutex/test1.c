/* verify that not calling pthread_*attr_destroy() causes memory leak */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

int main(int argc, char **argv)
{
	pthread_mutexattr_t mattr;
	pthread_condattr_t cattr;
	int err;

	for ( ; ; ) {
		if ((err = pthread_mutexattr_init(&mattr)) != 0) {
			fprintf(stderr, "pthread_mutexattr_init error: %s\n", strerror(err));
			exit(1);
		}
		if ((err = pthread_condattr_init(&cattr)) != 0) {
			fprintf(stderr, "pthread_condattr_init error: %s\n", strerror(err));
			exit(1);
		}
	}

	exit(0);
}
