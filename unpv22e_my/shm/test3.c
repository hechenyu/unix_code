#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int
main(int argc, char **argv)
{
	int		fd, i, n, val, *ptr;
	pid_t	childpid;

	if (argc != 2) {
		fprintf(stderr, "usage: test3 <pathname>\n");
		exit(1);
	}

	unlink(argv[1]);		/* error OK */
	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	val = 9999;
	if (write(fd, &val, sizeof(int)) != sizeof(int)) {	/* sets file size too */
		perror("write error");
		exit(1);
	}

	if ((ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}
	*ptr = 1;

	if ((childpid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		if ((fd = open(argv[1], O_RDONLY)) == -1) {		/* child */
			fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
			exit(1);
		}
		for (i = 0; i < 10; i++) {
			if (lseek(fd, 0, SEEK_SET) == (off_t) -1) {
				perror("lseek error");
				exit(1);
			}
			if ((n = read(fd, &val, sizeof(int))) == -1) {
				perror("read error");
				exit(1);
			} else if (n != sizeof(int)) {
				fprintf(stderr, "read returned %d\n", n);
				exit(1);
			}
			printf("val = %d\n", val);
			sleep(1);
		}
		exit(0);
	}
	if (waitpid(childpid, NULL, 0) == -1) {
		perror("waitpid error");
		exit(1);
	}

	exit(0);
}
