#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

static int	servfd;
static int	nsec;			/* #seconds between each alarm */
static int	maxnalarms;		/* #alarms w/no client probe before quit */
static int	nprobes;		/* #alarms since last client probe */
static void	sig_urg(int), sig_alrm(int);

void
heartbeat_serv(int servfd_arg, int nsec_arg, int maxnalarms_arg)
{
	servfd = servfd_arg;		/* set globals for signal handlers */
	if ( (nsec = nsec_arg) < 1)
		nsec = 1;
	if ( (maxnalarms = maxnalarms_arg) < nsec)
		maxnalarms = nsec;

	if (signal(SIGURG, sig_urg) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	if (fcntl(servfd, F_SETOWN, getpid()) == -1) {
		perror("fcntl error");
		exit(1);
	}

	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	alarm(nsec);
}

static void
sig_urg(int signo)
{
	int		n;
	char	c;
 
	if ( (n = recv(servfd, &c, 1, MSG_OOB)) < 0) {
		if (errno != EWOULDBLOCK) {
			perror("recv error");
			exit(1);
		}
	}
	if (send(servfd, &c, 1, MSG_OOB) != 1) {	/* echo back out-of-band byte */
		perror("send error");
		exit(1);
	}

	nprobes = 0;			/* reset counter */
	return;					/* may interrupt server code */
}

static void
sig_alrm(int signo)
{
	if (++nprobes > maxnalarms) {
		printf("no probes from client\n");
		exit(0);
	}
	alarm(nsec);
	return;					/* may interrupt server code */
}
