#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define	MAXLINE	4096

void client(int readfd, int writefd)
{
	size_t len;
	ssize_t	n;
	char buff[MAXLINE];

	/* 4read pathname */
	if (fgets(buff, MAXLINE, stdin) == NULL && ferror(stdin)) {
		perror("fgets error");
		exit(1);
	}
	len = strlen(buff);		/* fgets() guarantees null byte at end */
	if (buff[len-1] == '\n')
		len--;				/* delete newline from fgets() */

	/* 4write pathname to IPC channel */
	if (write(writefd, buff, len) != len) {
		perror("write error");
		exit(1);
	}

	/* 4read from IPC, write to standard output */
	while (1) {
		if ((n = read(readfd, buff, MAXLINE)) < 0) {
			perror("read error");
			exit(1);
		}
		if (n == 0)
			break;
		if (write(STDOUT_FILENO, buff, n) != n) {
			perror("write error");
			exit(1);
		}
	}
}

