/* 程序8-15 打印实际和有效用户ID */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());
	exit(0);
}

