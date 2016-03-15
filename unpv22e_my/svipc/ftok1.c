#include <sys/stat.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	struct stat	buf;
	key_t key;
	int id = 0x57;

	if (argc != 2) {
		fprintf(stderr, "usage: ftok1 <pathname>\n");
		exit(1);
	}

	if (stat(argv[1], &buf) == -1) {
		perror("stat error");
		exit(1);
	}
	if ((key = ftok(argv[1], id)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[1], id, strerror(errno));
		exit(1);
	}
	printf("%x %s\n", key, argv[1]);

	exit(0);
}
