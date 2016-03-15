/* 程序12-5 精灵进程阻止多份副本同时运行的启动代码 */
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define	PIDFILE	"daemon.pid"
#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define	write_lock(fd, offset, whence, len) \
			lock_reg(fd, F_SETLWK, F_WRLCK, offset, whence, len)

int lock_reg(int, int, int, off_t, int, off_t);

int main(void)
{
	int fd, val;
	char buf[10];

	if ((fd = open(FIDFILE, O_WRONLY | O_CREAT, FILE_MODE)) < 0) {
		perror("open error");
		exit(1);
	}

				/* try and set a write lock on the entire file */
	if (write_lock(fd, 0, SEEK_SET, 0) < 0) {
		if (errno == EACCES || errno == EAGAIN)
			exit(0);		/*  gracefully exit, deamon is already running */
		else {
			perror("write_lock error");
			exit(1);
		}
	}

				/* truncate to zero length, now that we have the lock */
	if (ftruncate(fd, 0) < 0) {
		perror("ftruncate error");
		exit(1);
	}

				/* and write our process ID */
	sprintf(buf, "%d\n", getpid());
	if (write(fd, buf, strlen(buf)) != strlen(buf)) {
		perror("write error");
		exit(1);
	}

				/* set close-on-exec flag for descriptor */
	if ((val = fcntl(fd, F_GETFD, 0)) < 0) {
		perror("fcntl F_GETFD error");
		exit(1);
	}
	val |= FD_CLOEXEC;
	if (fcntl(fd, F_SETFD, val) < 0) {
		perror("fcntl F_SETFD error");
		exit(1);
	}

	/* leave file open until we terminate: lock will be held */

	/* do whatever */

	exit(0);
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
