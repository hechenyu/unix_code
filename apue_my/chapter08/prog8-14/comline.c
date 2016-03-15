/* 程序8-14 用system执行命令行参数 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int status;

	if (argc < 2) {
		perror("command-line argument required");
		exit(1);
	}

	if ((status = system(argv[1])) < 0) {
		perror("system() error");
		exit(1);
	}
	pr_exit(status);

	exit(0);
}
