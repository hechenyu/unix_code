#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sum.h"

ssize_t
writen(int fd, const void *vptr, size_t n);

ssize_t
readn(int fd, void *vptr, size_t n);

void
str_echo(int sockfd)
{
	ssize_t			n;
	struct args		args;
	struct result	result;

	for ( ; ; ) {
		if ( (n = readn(sockfd, &args, sizeof(args))) < 0) {
			perror("readn error");
			exit(1);
		} else if (n == 0) {
			return;		/* connection closed by other end */
		}

		result.sum = args.arg1 + args.arg2;
		if (writen(sockfd, &result, sizeof(result)) != sizeof(result)) {
			perror("writen error");
			exit(1);
		}
	}
}
