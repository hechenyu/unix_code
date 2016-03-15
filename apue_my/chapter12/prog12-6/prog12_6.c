/* 程序12-6 显示相对于文件末尾的锁的问题的程序 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define writew_lock(fd, offset, whence, len) \
            lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define	un_lock(fd, offset, whence, len) \
			lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

int main(void)
{
	int i, fd;
	char buf[2] = "a";

	if ((fd = open("temp.lock", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
		perror("open error");
		exit(1);
	}

	for (i = 0; i < 1000000; i++) {		/* try to write 2 Mbytes */
				/* lock from current EOF to EOF */
		if (writew_lock(fd, 0, SEEK_END, 0) < 0) {
			perror("writew_lock error");
			exit(1);
		}

		if (write(fd, buf, 1) != 1) {
			perror("write error");
			exit(1);
		}

		if (un_lock(fd, 0, SEEK_END, 0) < 0) {
			perror("un_lock error");
			exit(1);
		}

		if (write(fd, buf, 1) != 1) {
			perror("write error");
			exit(1);
		}
	}
	exit(0);
}

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;     /* F_RDLCK, F_WRLCK, F_UNLCK */
    lock.l_start = offset;  /* byet offset, relative to l_whence */
    lock.l_whence = whence; /* SEEK_SET,  SEEK_CUR, SEEK_END */
    lock.l_len = len;       /* #bytes (0 means to EOF) */

    return(fcntl(fd, cmd, &lock));
}

