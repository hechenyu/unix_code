/* 程序8-7 修改程序8-6以避免竞态条件 */
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void charatatime(char *);
static void sig_usr(int);
void TELL_WAIT(void);
void TELL_PARENT(pid_t);
void WAIT_PARENT(void);
void TELL_CHILD(pid_t);
void WAIT_CHILD(void);

int main(void)
{
	pid_t pid;

	TELL_WAIT();

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {
		WAIT_PARENT();		/* parent goes first */
		charatatime("output from child\n");
	} else {
		charatatime("output from parent\n");
		TELL_CHILD(pid);
	}
	exit(0);
}

static void charatatime(char *str)
{
	char *ptr;
	int c;

	setbuf(stdout, NULL);		/* set unbuffered */
	for (ptr = str; c = *ptr++; )
		putc(c, stdout);
}

static volatile sig_atomic_t sigflag;
							/* set nonzero by signal handler */
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo)		/* one signal handler for SIGUSR1 and SIGUSR2 */
{
	sigflag = 1;
	return;
}

void TELL_WAIT(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
		perror("signal(SIGUSR1) error");
		exit(1);
	}
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
		perror("signal(SIGUSR2) error");
		exit(1);
	}

	sigemptyset(&zeromask);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1); 
	sigaddset(&newmask, SIGUSR2);
			/* block SIGUSR1 and SIGUSR2, and save current signal mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		perror("SIG_BLOCK error");
		exit(1);
	}
}

void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR2);			/* tell parent we're done */
}

void WAIT_PARENT(void)
{
	while (sigflag == 0)
		sigsuspend(&zeromask);		/* and wait for parent */

	sigflag = 0;
			/* reset signal mask to original value */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		perror("SIG_SETMASK error");
	}
}

void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);			/* tell child we're done */
}

void WAIT_CHILD(void)
{
	while(sigflag == 0)
		sigsuspend(&zeromask);			/* and wait for child */

	sigflag = 0;
			/* reset signal mask to original value */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		perror("SIG_SETMASK error");
		exit(1);
	}
}
