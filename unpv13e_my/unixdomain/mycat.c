#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define	BUFFSIZE	8192	/* buffer size for reads and writes */

int		my_open(const char *, int);

int
main(int argc, char **argv)
{
	int		fd, n;
	char	buff[BUFFSIZE];

	if (argc != 2) {
		fprintf(stderr, "usage: mycat <pathname>\n");
		exit(1);
	}

	if ( (fd = my_open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "cannot open %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	while ( (n = read(fd, buff, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buff, n) != n) {
			perror("write error");
			exit(1);
		}
	}
	if (n < 0) {
		perror("read error");
		exit(1);
	}

	exit(0);
}
