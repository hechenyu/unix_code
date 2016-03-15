/* 程序14-12 在父、子进程间使用/dev/zero存储映射I/O的IPC */
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	NLOOPS	1000
#define	SIZE	sizeof(long)		/* size of shared memory area */

static void sig_usr(int);
void TELL_WAIT(void);
void TELL_PARENT(pid_t);
void WAIT_PARENT(void);
void TELL_CHILD(pid_t);
void WAIT_CHILD(void);
static int update(long *);

int main()
{
	int fd, i, counter;
	pid_t pid;
	caddr_t area;

	if ((fd = open("/dev/zero", O_RDWR)) < 0) {
		perror("open error");
		exit(1);
	}
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE,
						MAP_SHARED, fd, 0)) == (caddr_t) -1) {
		perror("mmap error");
		exit(1);
	}
	close(fd);		/* can close /dev/zero now that it's mapped */

	TELL_WAIT();
	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {		/* parent */
		for (i = 0; i < NLOOPS; i += 2) {
			if ((counter = update((long *) area)) != i) {
				fprintf(stderr, "parent: expected %d, got %d", i, counter);
				exit(1);
			}
			fprintf(stderr, "parent:\tcounter = %d\n", i);
			TELL_CHILD(pid);
			WAIT_CHILD();
		}
	} else {	/* child */
		for (i = 1; i < NLOOPS + 1; i += 2) {
			WAIT_PARENT();
			if ((counter = update((long *) area)) != i) {
				fprintf(stderr, "child: expected %d, got %d", i, counter);
				exit(1);
			}
			fprintf(stderr, "child:\tcounter = %d\n", i);
			TELL_PARENT(getppid());
		}
	}
	exit(0);
}

static int update(long *ptr)
{
	return((*ptr)++);		/* return value before increment */
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
