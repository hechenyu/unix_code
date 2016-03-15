/* 程序3-2 创建一个具有空洞的文件 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
	int fd;

	if ((fd = creat("file.hole", FILE_MODE)) < 0) {
		perror("creat error");
		exit(1);
	}

	if (write(fd, buf1, 10) != 10) {
		perror("buf1 write error");
		exit(1);
	}
	/* offset now = 10 */

	if (lseek(fd, 40, SEEK_SET) == -1) {
		perror("lseek error");
		exit(1);
	}
	/* offset now = 40 */

	if (write(fd, buf2, 10) != 10) {
		perror("buf2 write error");
		exit(1);
	}
	/* offset now = 50 */

	exit(0);
}
		
		
