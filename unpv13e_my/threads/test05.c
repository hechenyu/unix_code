/* See what the implementation returns for TSD keys */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

pthread_key_t	my_key;

int
main(int argc, char **argv)
{
	int		*iptr;
	int		n;

	if ((n = pthread_key_create(&my_key, NULL)) != 0) {
		errno = n;
		perror("pthread_key_create error");
		exit(1);
	}
	printf("first key = %d\n", my_key);

	if ((n = pthread_key_create(&my_key, NULL)) != 0) {
		errno = n;
		perror("pthread_key_create error");
		exit(1);
	}
	printf("second key = %d\n", my_key);

	if ((n = pthread_key_create(&my_key, NULL)) != 0) {
		errno = n;
		perror("pthread_key_create error");
		exit(1);
	}
	printf("third key = %d\n", my_key);

	if ( (iptr = pthread_getspecific((pthread_key_t) 0)) == NULL)
		printf("key 0 pointer is NULL\n");
	else
		printf("value in key 0 = %d\n", *iptr);

	errno = 67;
	if ( (iptr = pthread_getspecific((pthread_key_t) 0)) == NULL)
		printf("key 0 pointer is NULL\n");
	else
		printf("value in key 0 = %d\n", *iptr);

	exit(0);
}
