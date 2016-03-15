/* 程序10-15 保护临界区不被信号中断 */
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_int(int);
void pr_mask(const char *);

int main(void)
{
	sigset_t newmask, oldmask, zeromask;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal(SIGINT) error");
		exit(1);
	}

	sigemptyset(&zeromask);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);
						/* block SIGINT and save current signal mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		perror("SIG_BLOCK error");
		exit(1);
	}

						/* critical region of code */
	pr_mask("in critical region: ");

						/* allow all signals and pause */
	if (sigsuspend(&zeromask) != -1) {
		perror("sigsuspend error");
		exit(1);
	}
	pr_mask("after return from sigsuspend: ");

						/* reset signal mask which unblocks SIGINT */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		perror("SIG_SETMASK error");
		exit(1);
	}

						/* and continue processing ... */
	exit(0);
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
	if (sigismember(&sigset, SIGABRT))
		printf("SIGABRT ");

	/* remaining signals can go here */
	printf("\n");
	errno = errno_save;
}

static void sig_int(int signo)
{
	pr_mask("\nin sig_int: ");
	return;
}
