/* 程序5-1 用getc和putc将标准输入复制到标准输出 */
#include <stdio.h>

int main(void)
{
	int c;

	while ((c = getc(stdin)) != EOF)
		if (putc(c, stdout) == EOF) {
			perror("output error");
			exit(1);
		}

	if (ferror(stdin)) {
		perror("input error");
		exit(1);
	}

	exit(0);
}
