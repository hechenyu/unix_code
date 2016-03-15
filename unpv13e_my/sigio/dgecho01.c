/* include dgecho1 */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

static int		sockfd;

#define	QSIZE	   8		/* size of input queue */
#define	MAXDG	4096		/* max datagram size */

typedef struct {
  void		*dg_data;		/* ptr to actual datagram */
  size_t	dg_len;			/* length of datagram */
  struct sockaddr  *dg_sa;	/* ptr to sockaddr{} w/client's address */
  socklen_t	dg_salen;		/* length of sockaddr{} */
} DG;
static DG	dg[QSIZE];			/* queue of datagrams to process */
static long	cntread[QSIZE+1];	/* diagnostic counter */

static int	iget;		/* next one for main loop to process */
static int	iput;		/* next one for signal handler to read into */
static int	nqueue;		/* # on queue for main loop to process */
static socklen_t clilen;/* max length of sockaddr{} */

static void	sig_io(int);
static void	sig_hup(int);
/* end dgecho1 */

/* include dgecho2 */
void
dg_echo(int sockfd_arg, SA *pcliaddr, socklen_t clilen_arg)
{
	int			i;
	const int	on = 1;
	sigset_t	zeromask, newmask, oldmask;

	sockfd = sockfd_arg;
	clilen = clilen_arg;

	for (i = 0; i < QSIZE; i++) {	/* init queue of buffers */
		if ((dg[i].dg_data = malloc(MAXDG)) == NULL) {
			perror("malloc error");
			exit(1);
		}
		if ((dg[i].dg_sa = malloc(clilen)) == NULL) {
			perror("malloc error");
			exit(1);
		}
		dg[i].dg_salen = clilen;
	}
	iget = iput = nqueue = 0;

	if (signal(SIGHUP, sig_hup) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	if (signal(SIGIO, sig_io) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	if (fcntl(sockfd, F_SETOWN, getpid()) == -1) {
		perror("fcntl error");
		exit(1);
	}
	if (ioctl(sockfd, FIOASYNC, &on) == -1) {
		perror("ioctl error");
		exit(1);
	}
	if (ioctl(sockfd, FIONBIO, &on) == -1) {
		perror("ioctl error");
		exit(1);
	}

	if (sigemptyset(&zeromask) == -1) {		/* init three signal sets */
		perror("sigemptyset error");
		exit(1);
	}
	if (sigemptyset(&oldmask) == -1) {
		perror("sigemptyset error");
		exit(1);
	}
	if (sigemptyset(&newmask) == -1) {
		perror("sigemptyset error");
		exit(1);
	}
	if (sigaddset(&newmask, SIGIO) == -1) {	/* signal we want to block */
		perror("sigaddset error");
		exit(1);
	}

	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1) {
		perror("sigprocmask error");
		exit(1);
	}
	for ( ; ; ) {
		while (nqueue == 0)
			sigsuspend(&zeromask);	/* wait for datagram to process */

			/* 4unblock SIGIO */
		if (sigprocmask(SIG_SETMASK, &oldmask, NULL) == -1) {
			perror("sigprocmask error");
			exit(1);
		}

		if (sendto(sockfd, dg[iget].dg_data, dg[iget].dg_len, 0,
			   dg[iget].dg_sa, dg[iget].dg_salen) != dg[iget].dg_len) {
			perror("sendto error");
			exit(1);
		}

		if (++iget >= QSIZE)
			iget = 0;

			/* 4block SIGIO */
		if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1) {
			perror("sigprocmask error");
			exit(1);
		}
		nqueue--;
	}
}
/* end dgecho2 */

/* include sig_io */
static void
sig_io(int signo)
{
	ssize_t		len;
	int			nread;
	DG			*ptr;

	for (nread = 0; ; ) {
		if (nqueue >= QSIZE) {
			fprintf(stderr, "receive overflow\n");
			exit(1);
		}

		ptr = &dg[iput];
		ptr->dg_salen = clilen;
		len = recvfrom(sockfd, ptr->dg_data, MAXDG, 0,
					   ptr->dg_sa, &ptr->dg_salen);
		if (len < 0) {
			if (errno == EWOULDBLOCK) {
				break;		/* all done; no more queued to read */
			} else {
				perror("recvfrom error");
				exit(1);
			}
		}
		ptr->dg_len = len;

		nread++;
		nqueue++;
		if (++iput >= QSIZE)
			iput = 0;

	}
	cntread[nread]++;		/* histogram of # datagrams read per signal */
}
/* end sig_io */

/* include sig_hup */
static void
sig_hup(int signo)
{
	int		i;

	for (i = 0; i <= QSIZE; i++)
		printf("cntread[%d] = %ld\n", i, cntread[i]);
}
/* end sig_hup */
