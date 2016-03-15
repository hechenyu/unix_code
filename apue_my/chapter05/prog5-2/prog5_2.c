/* 程序5-2 用fgets和fputs将标准输入复制到标准输出 */
#include <stdio.h>

#define	MAXLINE	4096

int main(void)
{
	char buf[MAXLINE];

	while (fgets(buf, MAXLINE, stdin) != NULL)
		if (fputs(buf, stdout) == EOF) {
			perror("output error");
			exit(1);
		}

	if (ferror(stdin)) {
		perror("input error");
		exit(1);
	}

	exit(0);
}
