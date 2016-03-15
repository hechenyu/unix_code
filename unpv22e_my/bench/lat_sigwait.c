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
	int		i, nloop, signo;
	pid_t	childpid, parentpid;
	sigset_t	newmask;
	int err;
	double st;	// stop time

	if (argc != 2) {
		fprintf(stderr, "usage: lat_sigwait <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);

	if (sigemptyset(&newmask) == -1) {
		perror("sigemptyset error");
		exit(1);
	}
	if (sigaddset(&newmask, SIGUSR1) == -1) {
		perror("sigaddset error");
		exit(1);
	}
	if (sigprocmask(SIG_BLOCK, &newmask, NULL) == -1) {		/* block SIGUSR1 */
		perror("sigprocmask error");
		exit(1);
	}

	parentpid = getpid();
	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		for (i = 0; i < nloop; i++) {		/* child */
			if ((err = sigwait(&newmask, &signo)) != 0) {
				fprintf(stderr, "sigwait error: %s\n", strerror(err));
				exit(1);
			}
			if (kill(parentpid, SIGUSR1) == -1) {
				perror("kill error");
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
		if (kill(childpid, SIGUSR1) == -1) {
				perror("kill error");
				exit(1);
		}
		if ((err = sigwait(&newmask, &signo)) != 0) {
			fprintf(stderr, "sigwait error: %s\n", strerror(err));
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
