#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	MAXLINE	4096

int main(int argc, char **argv)
{
	size_t	n;
	char buff[MAXLINE], command[MAXLINE];
	FILE *fp;

	/* 4read pathname */
	if (fgets(buff, MAXLINE, stdin) == NULL && ferror(stdin)) {
		perror("fgets error");
		exit(1);
	}
	n = strlen(buff);		/* fgets() guarantees null byte at end */
	if (buff[n-1] == '\n')
		n--;				/* delete newline from fgets() */

	snprintf(command, sizeof(command), "cat %s", buff);
	if ((fp = popen(command, "r")) == NULL) {
		fprintf(stderr, "popen error for %s: %s\n", command, strerror(errno));
		exit(1);
	}

	/* 4copy from pipe to standard output */
	while (1) {
		if (fgets(buff, MAXLINE, fp) == NULL) {
			if (ferror(fp)) {
				perror("fgets error");
				exit(1);
			} else {
				break;
			}
		}
		if (fputs(buff, stdout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}

	if (pclose(fp) == -1) {
		perror("pclose error");
		exit(1);
	}
	exit(0);
}
