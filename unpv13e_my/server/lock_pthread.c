/* include my_lock_init */
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static pthread_mutex_t	*mptr;	/* actual mutex will be in shared memory */

void
my_lock_init(char *pathname)
{
	int		fd;
	pthread_mutexattr_t	mattr;
	int		n;

	if ((fd = open("/dev/zero", O_RDWR, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", "/dev/zero", strerror(errno));
		exit(1);
	}

	if ((mptr = mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
				MAP_SHARED, fd, 0)) == (void *) -1) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}

	if ((n = pthread_mutexattr_init(&mattr)) != 0) {
		errno = n;
		perror("pthread_mutexattr_init error");
		exit(1);
	}
	if ((n = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED)) != 0) {
		errno = n;
		perror("pthread_mutexattr_setpshared error");
		exit(1);
	}
	if ((n = pthread_mutex_init(mptr, &mattr)) != 0) {
		errno = n;
		perror("pthread_mutex_init error");
		exit(1);
	}
}
/* end my_lock_init */

/* include my_lock_wait */
void
my_lock_wait()
{
	int n;
	if ((n = pthread_mutex_lock(mptr)) != 0) {
		errno = n;
		perror("pthread_mutex_lock error");
		exit(1);
	}
}

void
my_lock_release()
{
	int n;
	if ((n = pthread_mutex_unlock(mptr) != 0)) {
		errno = n;
		perror("pthread_mutex_unlock error");
		exit(1);
	}
}
/* end my_lock_wait */
