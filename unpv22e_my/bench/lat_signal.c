#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int
start_time(void);

double
stop_time(void);

static int		counter, nloop;
static pid_t	childpid, parentpid;

void
sig_usr1(int signo)
{
	if (kill(parentpid, SIGUSR2) == -1) {		/* child receives USR1, sends USR2 */
		perror("kill error");
		exit(1);
	}
	return;
}

void
sig_usr2(int signo)
{
	if (++counter < nloop) {
		if (kill(childpid, SIGUSR1) == -1) {	/* parent receives USR2, sends USR1 */
			perror("kill error");
			exit(1);
		}
	} else {
		if (kill(parentpid, SIGTERM) == -1) {	/* parent terminates below */
			perror("kill error");
			exit(1);
		}
	}
	return;
}

void
sig_term(int signo)
{
	double st;	// stop time
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("latency: %.3f usec\n", st / nloop);
	if (kill(childpid, SIGTERM) == -1) {
		perror("kill error");
		exit(1);
	}
	exit(0);
}

int
main(int argc, char **argv)
{

	if (argc != 2) {
		fprintf(stderr, "usage: lat_signal <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);
	counter = 0;

	parentpid = getpid();
	if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {		/* for child */
		perror("signal error");
		exit(1);
	}
	if (signal(SIGUSR2, sig_usr2) == SIG_ERR) {		/* for parent */
		perror("signal error");
		exit(1);
	}
	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		for ( ; ; ) {		/* child */
			pause();
		}
		exit(0);	/* never reached */
	}
		/* 4parent */
	if (signal(SIGTERM, sig_term) == SIG_ERR) {		/* for parent only */
		perror("signal error");
		exit(1);
	}
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	if (kill(childpid, SIGUSR1) == -1) {
		perror("kill error");
		exit(1);
	}
	for ( ; ; )
		pause();
}
