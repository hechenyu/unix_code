/* 程序10-10 为进程打印信号屏蔽字 */
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

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

