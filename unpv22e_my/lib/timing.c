/* include start_time */
#include <sys/time.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

static struct timeval	tv_start, tv_stop;

void
tv_sub(struct timeval *out, struct timeval *in);

int
start_time(void)
{
	return(gettimeofday(&tv_start, NULL));
}

double
stop_time(void)
{
	double	clockus;

	if (gettimeofday(&tv_stop, NULL) == -1)
		return(0.0);
	tv_sub(&tv_stop, &tv_start);
	clockus = tv_stop.tv_sec * 1000000.0 + tv_stop.tv_usec;
	return(clockus);
}

int
touch(void *vptr, int nbytes)
{
	char	*cptr;
	static int	pagesize = 0;

	if (pagesize == 0) {
		errno = 0;
#ifdef	_SC_PAGESIZE
		if ((pagesize = sysconf(_SC_PAGESIZE)) == -1)
			return(-1);
#else
		pagesize = getpagesize();		/* BSD */
#endif
	}

	cptr = vptr;
	while (nbytes > 0) {
		*cptr = 1;
		cptr += pagesize;
		nbytes -= pagesize;
	}
	return(0);
}
/* end start_time */
