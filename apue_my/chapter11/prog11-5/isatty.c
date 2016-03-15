/* 程序11-4 POSIX.1 isatty函数的实现 */
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int isatty(int);

int main(void)
{
	printf("fd 0: %s\n", isatty(0) ? "tty" : "not a tty");
	printf("fd 1: %s\n", isatty(1) ? "tty" : "not a tty");
	printf("fd 2: %s\n", isatty(2) ? "tty" : "not a tty");
	exit(0);
}

int isatty(int fd)
{
	struct termios term;
	return(tcgetattr(fd, &term) != -1);
}
