/* verify that calling pthread_attr_destroy() avoids memory leak */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

int main(int argc, char **argv)
{
	pthread_attr_t attr;
	int err;

	for ( ; ; ) {
		if ((err = pthread_attr_init(&attr)) != 0) {
			fprintf(stderr, "pthread_attr_init error: %s\n", strerror(err));
			exit(1);
		}
		if ((err = pthread_attr_destroy(&attr)) != 0) {
			fprintf(stderr, "pthread_attr_destroy error: %s\n", strerror(err));
			exit(1);
		}
	}

	exit(0);
}
