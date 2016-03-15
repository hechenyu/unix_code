/* 程序12-7  确定是否支持强制性锁 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define writew_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define write_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define read_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define un_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

int lock_reg(int, int, int, off_t, int, off_t);
void set_fl(int, int);
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
	char buff[5];
	struct stat statbuf;

	if ((fd = open("templock", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
		perror("open error");
		exit(1);
	}
	if (write(fd, "abcdef", 6) != 6) {
		perror("write error");
		exit(1);
	}

		/* turn on set-group-ID and turn off group-execute */
	if (fstat(fd, &statbuf) < 0) {
		perror("fstat error");
		exit(1);
	}
	if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
		perror("fchmod error");
		exit(1);
	}

	TELL_WAIT();
	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {	/* parent */
					/* write lock entire file */
		if (write_lock(fd, 0, SEEK_SET, 0) < 0) {
			perror("write_lock error");
			exit(1);
		}
		TELL_CHILD(pid);

		if (waitpid(pid, NULL, 0) < 0) {
			perror("waitpid error");
			exit(1);
		}
	} else {			/* child */
		WAIT_PARENT();	/* wait for parent to set lock */

		set_fl(fd, O_NONBLOCK);

			/* first let's see what error we get if region is locked */
		if (read_lock(fd, 0, SEEK_SET, 0) != -1) {	/* no wait */
			perror("child: read_lock succeeded");
			exit(1);
		}
		printf("read_lock of already-locked region returns %d\n", errno);

			/* now try to read the mandatory locked file */
		if (lseek(fd, 0, SEEK_SET) == -1) {
			perror("lseek error");
			exit(1);
		}
		if (read(fd, buff, 2) < 0)
			perror("read failed (mandatory locking works)");
		else
			printf("read OK (no mandatory locking), buff = %2.2s\n", buff);
	}
	exit(0);
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
		exit(1);
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

void set_fl(int fd, int flags)		/* flags are file status flags to turn on */
{
	int val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		perror("fcntl F_GETFL error");
		exit(1);
	}

	val |= flags;	/* turn on flags */

	if (fcntl(fd, F_SETFL, val) < 0) {
		perror("fcntl F_SETFL error");
		exit(1);
	}
}

