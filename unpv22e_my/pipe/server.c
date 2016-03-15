#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	MAXLINE	4096

void server(int readfd, int writefd)
{
	int fd;
	ssize_t	n;
	char buff[MAXLINE+1];

	/* 4read pathname from IPC channel */
	if ((n = read(readfd, buff, MAXLINE)) < 0) {
		perror("read error");
		exit(1);
	}
	if (n == 0) {
		printf("end-of-file while reading pathname\n");
		exit(1);
	}
	buff[n] = '\0';		/* null terminate pathname */

	if ((fd = open(buff, O_RDONLY)) < 0) {
		/* 4error: must tell client */
		snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n",
			 strerror(errno));
		n = strlen(buff);
		if (write(writefd, buff, n) != n) {
			fprintf(stderr, "write error: %s\n", strerror(errno));
			exit(1);
		}
	} else {
		/* 4open succeeded: copy file to IPC channel */
		while (1) {
			if ((n = read(fd, buff, MAXLINE)) < 0) {
				perror("read error");
				exit(1);
			}
			if (n == 0)
				break;
			if (write(writefd, buff, n) != n) {
				perror("write error");
				exit(1);
			}
		}
		if (close(fd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
