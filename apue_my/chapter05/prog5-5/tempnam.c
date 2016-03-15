/* 程序5-5 tempnam函数的应用 */
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		perror("usage: a.out <directory> <prefix>");
		exit(1);
	}

	printf("%s\n", tempnam(argv[1][0] != ' ' ? argv[1] : NULL,
							argv[2][0] != ' ' ? argv[2] : NULL));
	exit(0);
}

