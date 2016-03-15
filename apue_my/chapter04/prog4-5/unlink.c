/* 程序4-5 打开一个文件，然后unlink它 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	if (open("tempfile", O_RDWR) < 0) {
		perror("open error");
		exit(1);
	}

	if (unlink("tempfile") < 0) {
		perror("unlink error");
		exit(1);
	}

	printf("file unlinked\n");
	sleep(15);
	printf("done\n");

	exit(0);
}
