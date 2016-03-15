/* include serv07 */
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "pthread07.h"

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

pthread_mutex_t	mlock = PTHREAD_MUTEX_INITIALIZER;

int
main(int argc, char **argv)
{
	int		i;
	void	sig_int(int), thread_make(int);

	if (argc == 3) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 4) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: serv07 [ <host> ] <port#> <#threads>\n");
		exit(1);
	}
	nthreads = atoi(argv[argc-1]);
	if ((tptr = calloc(nthreads, sizeof(Thread))) == NULL) {
		perror("calloc error");
		exit(1);
	}

	for (i = 0; i < nthreads; i++)
		thread_make(i);			/* only main thread returns */

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	for ( ; ; )
		pause();	/* everything done by threads */
}
/* end serv07 */

void
sig_int(int signo)
{
	int		i;
	void	pr_cpu_time(void);

	pr_cpu_time();

	for (i = 0; i < nthreads; i++)
		printf("thread %d, %ld connections\n", i, tptr[i].thread_count);

	exit(0);
}
