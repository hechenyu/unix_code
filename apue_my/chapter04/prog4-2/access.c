/* ³ÌÐò4-2 accessº¯ÊýÊµÀý */
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		perror("usage: a.out <pathname>");
		exit(1);
	}

	if (access(argv[1], R_OK) < 0) 
		fprintf(stderr, "access error for %s\n", argv[1]);
	else
		printf("read access OK\n");

	if (open(argv[1], O_RDONLY) < 0) 
		fprintf(stderr, "open error for %s\n", argv[1]);
	else
		printf("open for reading OK\n");

	exit(0);
}

		
