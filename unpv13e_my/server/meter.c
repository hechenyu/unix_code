#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

/*
 * Allocate an array of "nchildren" longs in shared memory that can
 * be used as a counter by each child of how many clients it services.
 * See pp. 467-470 of "Advanced Programming in the Unix Environment."
 */

long *
meter(int nchildren)
{
	int		fd;
	long	*ptr;

#ifdef	MAP_ANON
	if ((ptr = mmap(0, nchildren*sizeof(long), PROT_READ | PROT_WRITE,
			   MAP_ANON | MAP_SHARED, -1, 0)) == (void *) -1) {
		perror("mmap error");
		exit(1);
	}
#else
	if ((fd = open("/dev/zero", O_RDWR, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", "/dev/zero", strerror(errno));
		exit(1);
	}

	if ((ptr = mmap(0, nchildren*sizeof(long), PROT_READ | PROT_WRITE,
			   MAP_SHARED, fd, 0)) == (void *) -1) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}
#endif

	return(ptr);
}
