#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE     4096    /* max text line length */

ssize_t
readline(int fd, void *vptr, size_t maxlen);

ssize_t
writen(int fd, const void *vptr, size_t n);

void
str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];
	int n;

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		n = strlen(sendline);
		if (writen(sockfd, sendline, n) != n) {
			perror("writen error");
			exit(1);
		}

		sleep(1);

		if (writen(sockfd, sendline, n) != n) {
			perror("writen error");
			exit(1);
		}

		if ((n = readline(sockfd, recvline, MAXLINE)) < 0) {
			perror("readline error");
			exit(1);
		} if (n == 0) {
			fprintf(stderr, "str_cli: server terminated prematurely\n");
			exit(1);
		}

		if (fputs(recvline, stdout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}

	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}
}
