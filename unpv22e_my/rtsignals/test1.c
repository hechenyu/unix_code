#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef	void Sigfunc_rt(int, siginfo_t *, void *);

Sigfunc_rt *
signal_rt(int signo, Sigfunc_rt *func);

static void	sig_rt(int, siginfo_t *, void *);

int
main(int argc, char **argv)
{
	int		i, j;
	pid_t	pid;
	sigset_t	newset;
	union sigval	val;

	printf("SIGRTMIN = %d, SIGRTMAX = %d\n", (int) SIGRTMIN, (int) SIGRTMAX);

	if ((pid = fork()) == -1) {
		perror("fork error");
		exit(1);
	}
	if (pid == 0) {
			/* 4child: block three realtime signals */
		if (sigemptyset(&newset) == -1) {
			perror("sigemptyset error");
			exit(1);
		}
		if (sigaddset(&newset, SIGRTMAX) == -1) {
			perror("sigaddset error");
			exit(1);
		}
		if (sigaddset(&newset, SIGRTMAX - 1) == -1) {
			perror("sigaddset error");
			exit(1);
		}
		if (sigaddset(&newset, SIGRTMAX - 2) == -1) {
			perror("sigaddset error");
			exit(1);
		}
		if (sigprocmask(SIG_BLOCK, &newset, NULL) == -1) {
			perror("sigprocmask error");
			exit(1);
		}

			/* 4establish signal handler with SA_SIGINFO set */
		if (signal_rt(SIGRTMAX, sig_rt) == (Sigfunc_rt *) SIG_ERR) {
			perror("signal_rt error");
			exit(1);
		}
		if (signal_rt(SIGRTMAX - 1, sig_rt) == (Sigfunc_rt *) SIG_ERR) {
			perror("signal_rt error");
			exit(1);
		}
		if (signal_rt(SIGRTMAX - 2, sig_rt) == (Sigfunc_rt *) SIG_ERR) {
			perror("signal_rt error");
			exit(1);
		}

		sleep(6);		/* let parent send all the signals */

		if (sigprocmask(SIG_UNBLOCK, &newset, NULL) == -1) {	/* unblock */
			perror("sigprocmask error");
			exit(1);
		}
		sleep(3);		/* let all queued signals be delivered */
		exit(0);
	}

		/* 4parent sends nine signals to child */
	sleep(3);		/* let child block all signals */
	for (i = SIGRTMAX; i >= SIGRTMAX - 2; i--) {
		for (j = 0; j <= 2; j++) {
			val.sival_int = j;
			if (sigqueue(pid, i, val) == -1) {
				perror("sigqueue error");
				exit(1);
			}
			printf("sent signal %d, val = %d\n", i, j);
		}
	}
	exit(0);
}

static void
sig_rt(int signo, siginfo_t *info, void *context)
{
	printf("received signal #%d, code = %d, ival = %d\n",
		   signo, info->si_code, info->si_value.sival_int);
}
