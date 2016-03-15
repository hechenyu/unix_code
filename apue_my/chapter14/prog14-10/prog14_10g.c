/* 程序14-10 对两个数求和的滤波程序，使用标准I/O */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	MAXLINE	4096

int main(void)
{
	int int1, int2;
	char line[MAXLINE];

	if (setvbuf(stdin, NULL, _IOLBF, 0) != 0) {
		perror("setvbuf error");
		exit(1);
	}
	if (setvbuf(stdout, NULL, _IOLBF, 0) != 0) {
		perror("setvbuf error");
		exit(1);
	}
	while (fgets(line, MAXLINE, stdin) != NULL) {
		if (sscanf(line, "%d%d", &int1, &int2) == 2) {
			if (printf("%d\n", int1 + int2) == EOF) {
				perror("printf error");
				exit(1);
			}
		} else {
			if (printf("invalid args\n") == EOF) {
				perror("printf error");
				exit(1);
			}
		}
	}
	exit(0);
}
