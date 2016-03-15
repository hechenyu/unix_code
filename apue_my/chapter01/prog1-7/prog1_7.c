/* 程序1-7 打印用户ID和组ID */
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	printf("uid = %d, gid = %d\n", getuid(), getgid());
	exit(0);
}
