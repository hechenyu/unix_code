/* 程序3-4 对于指定的描述符打印文件标志 */
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int accmode, val;

	if (argc != 2) {
		perror("usage: a.out <descriptor#>");
		exit(1);
	}

	if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
		fprintf(stderr, "fcntl error for fd %d", atoi(argv[1]));
		exit(1);
	}

	accmode = val & O_ACCMODE;
	if (accmode == O_RDONLY)
		printf("read only");
	else if (accmode == O_WRONLY)
		printf("write only");
	else if (accmode == O_RDWR)
		printf("read write");
	else {
		perror("unknown access mode");
		exit(1);
	}

	if (val & O_APPEND)
		printf(", append");
	if (val & O_NONBLOCK)
		printf(", nonblocking");
#if !defined(_POSIX_SOURCE) && defined(O_SYNC)
	if (val & O_SYNC)
		printf(", synchronous writes");
#endif
	putchar('\n');
	exit(0);
}
		
