/* 程序1-3 用标准I/O将标准输入复制到标准输出 */
#include <unistd.h>
#include <stdio.h>

void err_sys(char *, int);

int main(void)
{
	int c;

	while ((c=getc(stdin)) != EOF)
		if (putc(c, stdout) == EOF) 
			err_sys("输出错误!", 1);			

	if (ferror(stdin))
		err_sys("输入错误!", 2);

	exit(0);
}

void err_sys(char *str, int errno)
{
	fprintf(stderr, str);
	exit(errno);
}
