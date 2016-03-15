/*
 * See if signals other than the realtime signals are queued,
 * and if SA_SIGINFO works with these other signals.
 */
#include <sys/wait.h>
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
	int		j;
	pid_t	pid;
	sigset_t	newset, oldset;
	union sigval	val;

	if ((pid = fork()) == -1) {
		perror("fork error");
		exit(1);
	}
	if (pid == 0) {
			/* 4block all signals except SIGALRM */
		if (sigfillset(&newset) == -1) {
			perror("sigfillset error");
			exit(1);
		}
		if (sigdelset(&newset, SIGALRM) == -1) {
			perror("sigdelset error");
			exit(1);
		}
		if (sigprocmask(SIG_SETMASK, &newset, &oldset) == -1) {
			perror("sigprocmask error");
			exit(1);
		}

			/* establish signal handler with SA_SIGINFO set */
		if (signal_rt(SIGUSR1, sig_rt) == (Sigfunc_rt *) SIG_ERR) {
			perror("signal_rt error");
			exit(1);
		}
		if (signal_rt(SIGTERM, sig_rt) == (Sigfunc_rt *) SIG_ERR) {
			perror("signal_rt error");
			exit(1);
		}

		sleep(6);		/* let parent send all the signals */

		if (sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) {	/* unblock */
			perror("sigprocmask error");
			exit(1);
		}
		sleep(3);		/* let all queued signals be delivered */
		exit(0);
	}

	sleep(3);		/* let child block all signals */
		/* 4parent sends 12 signals to child */
	for (j = 0; j <= 6; j++) {
		val.sival_int = j;
		if (sigqueue(pid, SIGUSR1, val) == -1) {
			perror("sigqueue error");
			exit(1);
		}
		printf("sent SIGUSR1 (%d), val = %d\n", SIGUSR1, j);
	}
	for (j = 0; j <= 6; j++) {
		val.sival_int = j;
		if (sigqueue(pid, SIGTERM, val) == -1) {
			perror("sigqueue error");
			exit(1);
		}
		printf("sent SIGTERM (%d), val = %d\n", SIGTERM, j);
	}
	if (waitpid(pid, NULL, 0) == -1) {		/* wait for child */
		perror("waitpid error");
		exit(1);
	}
	exit(0);
}

static void
sig_rt(int signo, siginfo_t *info, void *context)
{
	printf("received signal #%d, code = %d, ival = %d\n",
		   signo, info->si_code, info->si_value.sival_int);
}
