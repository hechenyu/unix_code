#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv)
{
	int fd1, fd2, *ptr1, *ptr2;
	pid_t childpid;
	struct stat stat;

	if (argc != 2) {
		fprintf(stderr, "usage: test3 <name>\n");
		exit(1);
	}

	shm_unlink(argv[1]);
	if ((fd1 = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, FILE_MODE)) == -1) {
		fprintf(stderr, "shm_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (ftruncate(fd1, sizeof(int)) == -1) {
		perror("ftruncate error");
		exit(1);
	}
	if ((fd2 = open("/etc/passwd", O_RDONLY)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", "/etc/passwd", strerror(errno));
		exit(1);
	}
	if (fstat(fd2, &stat) == -1) {
		perror("fstat error");
		exit(1);
	}

	childpid = fork();
	if (childpid == -1) {
		perror("fork error");
		exit(1);
	}
	if (childpid == 0) {
		/* 4child */
		if ((ptr2 = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd2, 0)) == MAP_FAILED) {
			perror("mmap error");
			exit(1);
		}
		if ((ptr1 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0)) == MAP_FAILED) {
			perror("mmap error");
			exit(1);
		}
		printf("child: shm ptr = %p, passwd ptr = %p\n", ptr1, ptr2);

		sleep(5);
		printf("shared memory integer = %d\n", *ptr1);
		exit(0);
	}

	/* 4parent: mmap in reverse order from child */
	if ((ptr1 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if ((ptr2 = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd2, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	printf("parent: shm ptr = %p, passwd ptr = %p\n", ptr1, ptr2);
	*ptr1 = 777;
	if (waitpid(childpid, NULL, 0) == -1) {
		perror("waitpid error");
		exit(1);
	}

	exit(0);
}
