#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAXLINE     4096    /* max text line length */

void	*copyto(void *);

static int	sockfd;	
static FILE	*fp;
static int	done;

ssize_t
readline(int fd, void *vptr, size_t maxlen);

ssize_t
writen(int fd, const void *vptr, size_t n);

void
str_cli(FILE *fp_arg, int sockfd_arg)
{
	char		recvline[MAXLINE];
	pthread_t	tid;
	int n;

	sockfd = sockfd_arg;	/* copy arguments to externals */
	fp = fp_arg;

	if ((n = pthread_create(&tid, NULL, copyto, NULL)) != 0) {
		errno = n;
		perror("pthread_create error");
		exit(1);
	}

	while ((n = readline(sockfd, recvline, MAXLINE)) > 0) {
		if (fputs(recvline, stdout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}
	if (n < 0) {
		perror("readline error");
		exit(1);
	}

	if (done == 0) {
		fprintf(stderr, "server terminated prematurely\n");
		exit(1);
	}
}

void *
copyto(void *arg)
{
	char	sendline[MAXLINE];
	int n;

	while (fgets(sendline, MAXLINE, fp) != NULL) {
		n = strlen(sendline);
		if (writen(sockfd, sendline, n) != n) {
			perror("writen error");
			exit(1);
		}
	}
	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}

	if (shutdown(sockfd, SHUT_WR) < 0) {	/* EOF on stdin, send FIN */
		perror("shutdown error");
		exit(1);
	}

	done = 1;
	return(NULL);
	/* return (i.e., thread terminates) when end-of-file on stdin */
}
