/* 程序1-2 将标准输入复制到标准输出 */
#include <unistd.h>
#include <stdio.h>

#define	BUFFSIZE	8192

int main(void)
{
	int n;
	char buf[BUFFSIZE];

	while ((n=read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n) {
			fprintf(stderr, "write error");
			exit(1);
		}

	if (n < 0) {
		fprintf(stderr, "read error");
		exit(2);
	}

	exit(0);
}



