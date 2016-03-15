/* include connect_timeo */
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

static void	connect_alarm(int);

typedef void    Sigfunc(int);   /* for signal handlers */

int
connect_timeo(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	Sigfunc	*sigfunc;
	int		n;

	if ((sigfunc = signal(SIGALRM, connect_alarm)) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	if (alarm(nsec) != 0) {
		fprintf(stderr, "connect_timeo: alarm was already set\n");
	}

	if ( (n = connect(sockfd, saptr, salen)) < 0) {
		close(sockfd);
		if (errno == EINTR)
			errno = ETIMEDOUT;
	}
	alarm(0);					/* turn off the alarm */
	if (signal(SIGALRM, sigfunc) == SIG_ERR) {	/* restore previous signal handler */
		perror("signal error");
		exit(1);
	}

	return(n);
}

static void
connect_alarm(int signo)
{
	return;		/* just interrupt the connect() */
}
/* end connect_timeo */

