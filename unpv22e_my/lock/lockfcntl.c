#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void my_lock(int fd)
{
	struct flock lock;

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;				/* write lock entire file */

	if (fcntl(fd, F_SETLKW, &lock) == -1) {
		printf("fcntl error: %s\n", strerror(errno));
		exit(1);
	}
}

void my_unlock(int fd)
{
	struct flock lock;

	lock.l_type = F_UNLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;				/* unlock entire file */

	if (fcntl(fd, F_SETLK, &lock) == -1) {
		printf("fcntl error: %s\n", strerror(errno));
		exit(1);
	}
}
