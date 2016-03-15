/* 程序14-4 用popen向分页程序传送文件 */
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 4096
#define	PAGER	"${PAGER:-more}"	/* environment variable, or default */

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	FILE *fpin, *fpout;

	if (argc != 2) {
		perror("usage: a.out <pathname>");
		exit(1);
	}

	if ((fpin = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "can't open %s", argv[1]);
		exit(1);
	}

	if ((fpout = popen(PAGER, "w")) == NULL) {
		perror("popen error");
		exit(1);
	}

		/* copy argv[1] to pager */
	while (fgets(line, MAXLINE, fpin) != NULL) {
		if (fputs(line, fpout) == EOF) {
			perror("fputs error to pipe");
			exit(1);
		}
	}
	if (ferror(fpin)) {
		perror("fgets error");
		exit(1);
	}
	if (pclose(fpout) == -1) {
		perror("pclose error");
		exit(1);
	}
	exit(0);
}
