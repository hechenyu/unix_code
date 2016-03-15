/* 程序14-7 调用大写/小写过滤程序以读取命令 */
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	MAXLINE	4096

int main(void)
{
	char line[MAXLINE];
	FILE *fpin;

	if ((fpin = popen("myuclc", "r")) == NULL) {
		perror("popen error");
		exit(1);
	}

	for ( ; ; ) {
		fputs("prompt> ", stdout);
		fflush(stdout);
		if (fgets(line, MAXLINE, fpin) == NULL)		/* read from pipe */
			break;
		if (fputs(line, stdout) == EOF) {
			perror("fputs error to pipe");
			exit(1);
		}
	}
	if (pclose(fpin) == -1) {
		perror("pclose error");
		exit(1);
	}
	putchar('\n');
	exit(0);
}
