#include <stdio.h>
#include <stdlib.h>

#define MAXLINE     4096    /* max text line length */

void
str_echo(int sockfd)
{
	char		line[MAXLINE];
	FILE		*fpin, *fpout;

	if ((fpin = fdopen(sockfd, "r")) == NULL) {
		perror("fdopen error");
		exit(1);
	}
	if ((fpout = fdopen(sockfd, "w")) == NULL) {
		perror("fdopen error");
		exit(1);
	}

	while (fgets(line, MAXLINE, fpin) != NULL) {
		if (fputs(line, fpout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}

	if (ferror(fpin)) {
		perror("fgets error");
		exit(1);
	}
}
