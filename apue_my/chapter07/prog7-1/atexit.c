/* 程序7-1 终止处理程序实例 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void my_exit1(void), my_exit2(void);

int main(void)
{
	if (atexit(my_exit2) != 0) {
		perror("can't register my_exit2");
		exit(1);
	}
	if (atexit(my_exit1) != 0) {
		perror("can't register my_exit1");
		exit(1);
	}
	if (atexit(my_exit1) != 0) {
		perror("can't register my_exit1");
		exit(1);
	}

	printf("main is done\n");
	return (0);
}

static void my_exit1(void)
{
	printf("first exit handler\n");
}

static void my_exit2(void)
{
	printf("second exit handler\n");
}

