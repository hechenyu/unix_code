#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define	BUFFSIZE	8192

int main(int argc, char **argv)
{
	int fd, n;
	char buff[BUFFSIZE];

	if (argc != 2) {
		fprintf(stderr, "usage: mycat1 <pathname>\n");
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	while (1) {
		n = read(fd, buff, BUFFSIZE);
		if (n < 0) {
			perror("read error");
			exit(1);
		}
		if (n == 0) {
			break;
		}
		if (write(STDOUT_FILENO, buff, n) != n) {
			perror("write error");
			exit(1);
		}
	} 

	exit(0);
}
