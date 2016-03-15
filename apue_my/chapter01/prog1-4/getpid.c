/* 程序1-4 打印进程ID */
#include <unistd.h>
#include <stdio.h>

int main(void) 
{
	printf("hello world from process ID %d\n", getpid());
	exit(0);
}

