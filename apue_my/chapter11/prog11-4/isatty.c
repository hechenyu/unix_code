/* 程序11-4 POSIX.1 isatty函数的实现 */
#include <termios.h>

int isatty(int fd)
{
	struct termios term;
	return(tcgetattr(fd, &term) != -1);
}
