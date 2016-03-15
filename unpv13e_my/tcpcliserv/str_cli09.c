#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sum.h"

#define MAXLINE     4096    /* max text line length */

ssize_t
writen(int fd, const void *vptr, size_t n);

ssize_t
readn(int fd, void *vptr, size_t n);

void
str_cli(FILE *fp, int sockfd)
{
	char			sendline[MAXLINE];
	struct args		args;
	struct result	result;
	int n;

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		if (sscanf(sendline, "%ld%ld", &args.arg1, &args.arg2) != 2) {
			printf("invalid input: %s", sendline);
			continue;
		}

		if (writen(sockfd, &args, sizeof(args)) != sizeof(args)) {
			perror("writen error");
			exit(1);
		}

		if ((n = readn(sockfd, &result, sizeof(result))) < 0) {
			perror("readn error");
			exit(1);
		} else if (n == 0) {
			fprintf(stderr, "str_cli: server terminated prematurely\n");
			exit(1);
		}

		printf("%ld\n", result.sum);
	}

	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}
}

