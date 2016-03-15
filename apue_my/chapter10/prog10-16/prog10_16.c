/* 程序10-16 用sigsuspend等待一个全局变量被设置 */
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

volatile sig_atomic_t quitflag;			/* set nonzero by signal handler */

int main(void)
{
	void sig_int(int);
	sigset_t newmask, oldmask, zeromask;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal(SIGINT) error");
		exit(1);
	}
	if (signal(SIGQUIT, sig_int) == SIG_ERR) {
		perror("signal(SIGQUIT) error");
		exit(1);
	}

	sigemptyset(&zeromask);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
						/* block SIGQUIT and save current signal mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		perror("SIG_BLOCK error");
		exit(1);
	}

	while (quitflag == 0)
		sigsuspend(&zeromask);

	/* SIGQUIT has been caught and is now blocked; do whatever */
	quitflag = 0;
						/* reset signal mask which unblocks SIGQUIT */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		perror("SIG_SETMASK error");
		exit(1);
	}

	exit(0);
}

void sig_int(int signo)			/* one signal handler for SIGINT and SIGQUIT */
{
	if (signo == SIGINT)
		printf("\ninterrupt\n");
	else if (signo == SIGQUIT)
		quitflag = 1;			/* set flag for main loop */
	return;
}

void pr_mask(const char *str)
{
	sigset_t sigset;
	int errno_save;

	errno_save = errno;			/* we can be called by signal handlers */
	if (sigprocmask(0, NULL, &sigset) < 0) {
		perror("sigprocmask error");
		exit(1);
	}

	printf("%s", str);
	if (sigismember(&sigset, SIGINT))
		printf("SIGINT ");
	if (sigismember(&sigset, SIGQUIT))
		printf("SIGQUIT ");
	if (sigismember(&sigset, SIGUSR1))
		printf("SIGUSR1 ");
	if (sigismember(&sigset, SIGALRM))
		printf("SIGALRM ");

	/* remaining signals can go here */
	printf("\n");
	errno = errno_save;
}

