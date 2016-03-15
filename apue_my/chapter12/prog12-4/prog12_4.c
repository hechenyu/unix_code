/* ³ÌÐò12-4 ËÀËø¼ì²âÊµÀý */
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define writew_lock(fd, offset, whence, len) \
			lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static void lockabyte(const char *, int, off_t);
int lock_reg(int, int, int, off_t, int, off_t);
static void charatatime(char *);
static void sig_usr(int);
void TELL_WAIT(void);
void TELL_PARENT(pid_t);
void WAIT_PARENT(void);
void TELL_CHILD(pid_t);
void WAIT_CHILD(void);

int main(void)
{
	int fd;
	pid_t pid;

		/* Create a file and write two bytes to it */
	if ((fd = creat("templock", FILE_MODE)) < 0) {
		perror("creat error");
		exit(1);
	}
	if (write(fd, "ab", 2) != 2) {
		perror("write error");
		exit(1);
	}

	TELL_WAIT();
	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}

	else if (pid == 0) {		/* child */
		lockabyte("child", fd, 0);
		TELL_PARENT(getppid());
		WAIT_PARENT();
		lockabyte("child", fd, 1);
	} else {					/* parent */
		lockabyte("parent", fd, 1);
		TELL_CHILD(pid);
		WAIT_CHILD();
		lockabyte("parent", fd, 0);
	}
	exit(0);
}

static void lockabyte(const char *name, int fd, off_t offset)
{
	if (writew_lock(fd, offset, SEEK_SET, 1) < 0) {
		fprintf(stderr, "%s: writew_lock error\n", name);
		exit(1);
	}

	printf("%s: got the lock, byte %d\n", name, offset);
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

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;

	lock.l_type = type;		/* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset;	/* byet offset, relative to l_whence */
	lock.l_whence = whence;	/* SEEK_SET,  SEEK_CUR, SEEK_END */
	lock.l_len = len;		/* #bytes (0 means to EOF) */

	return(fcntl(fd, cmd, &lock));
}
