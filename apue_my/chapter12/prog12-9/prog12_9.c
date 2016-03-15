/* 程序12-8 检查描述符是否引用流设备 */
#include <sys/types.h>
#include <fcntl.h>
#include <stropts.h>
#include <unistd.h>
#include <stdio.h>

int isastream(int);

int main(int argc, char *argv[])
{
	int i, fd;

	for (i = 1; i < argc; i++) {
		fprintf(stderr, "%s: ", argv[i]);
		if ((fd = open(argv[i], O_RDONLY)) < 0) {
			fprintf(stderr, "%s: can't open\n", argv[i]);	
			continue; 
		} else {
			fprintf(stderr, "%s: can open\n", argv[i]);
			continue;
		}

		if (isastream(fd) == 0)
			fprintf(stderr, "%s: not a stream\n", argv[i]);
		else
			fprintf(stderr, "%s: streams device\n", argv[i]);
	}
	exit(0);
}

int isastream(int fd)
{
	return(ioctl(fd, I_CANPUT, 0) != -1);
}
