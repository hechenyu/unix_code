/* include serv02 */
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

static int		nchildren;
static pid_t	*pids;

int
main(int argc, char **argv)
{
	int			listenfd, i;
	socklen_t	addrlen;
	void		sig_int(int);
	pid_t		child_make(int, int, int);

	if (argc == 3) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 4) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: serv02 [ <host> ] <port#> <#children>\n");
		exit(1);
	}
	nchildren = atoi(argv[argc-1]);
	if ((pids = calloc(nchildren, sizeof(pid_t))) == NULL) {
		perror("calloc error");
		exit(1);
	}

	for (i = 0; i < nchildren; i++)
		pids[i] = child_make(i, listenfd, addrlen);	/* parent returns */

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	for ( ; ; )
		pause();	/* everything done by children */
}
/* end serv02 */

/* include sigint */
void
sig_int(int signo)
{
	int		i;
	void	pr_cpu_time(void);

		/* 4terminate all children */
	for (i = 0; i < nchildren; i++)
		kill(pids[i], SIGTERM);
	while (wait(NULL) > 0)		/* wait for all children */
		;
	if (errno != ECHILD) {
		perror("wait error");
		exit(1);
	}

	pr_cpu_time();
	exit(0);
}
/* end sigint */
