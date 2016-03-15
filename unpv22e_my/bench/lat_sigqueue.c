#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
start_time(void);

double
stop_time(void);

int
main(int argc, char **argv)
{
	int		i, nloop;
	pid_t	childpid, parentpid;
	sigset_t	newmask;
	siginfo_t	siginfo;
	union sigval	val;
	double st;	// stop time

	if (argc != 2) {
		fprintf(stderr, "usage: lat_sigqueue <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);

	if (sigemptyset(&newmask) == -1) {
		perror("sigemptyset error");
		exit(1);
	}
	if (sigaddset(&newmask, SIGRTMIN) == -1) {
		perror("sigaddset error");
		exit(1);
	}
	if (sigprocmask(SIG_BLOCK, &newmask, NULL) == -1) {		/* block SIGRTMIN */
		perror("sigprocmask error");
		exit(1);
	}

	parentpid = getpid();
	val.sival_int = 0;
	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		for (i = 0; i < nloop; i++) {		/* child */
			if (sigwaitinfo(&newmask, &siginfo) == -1) {
				fprintf(stderr, "sigwaitinfo error, pid = %ld: %s\n", (long) getpid(), strerror(errno));
				exit(1);
			}
			if (sigqueue(parentpid, SIGRTMIN, val) == -1) {
				perror("sigqueue error");
				exit(1);
			}
		}
		exit(0);
	}
		/* 4parent */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	for (i = 0; i < nloop; i++) {
		if (sigqueue(childpid, SIGRTMIN, val) == -1) {
			perror("sigqueue error");
			exit(1);
		}
		if (sigwaitinfo(&newmask, &siginfo) == -1) {
			fprintf(stderr, "sigwaitinfo error, pid = %ld: %s\n", (long) getpid(), strerror(errno));
			exit(1);
		}
	}
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("latency: %.3f usec\n", st / nloop);
	exit(0);
}
