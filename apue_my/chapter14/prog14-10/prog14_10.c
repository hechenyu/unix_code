/* 程序14-10 对两个数求和的滤波程序，使用标准I/O */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	MAXLINE	4096

int main(void)
{
	int int1, int2;
	char line[MAXLINE];

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
