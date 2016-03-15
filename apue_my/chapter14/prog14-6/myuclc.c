/* 程序14-6 过滤程序，将大写字符变成小写字符 */
#include <ctype.h>
#include <stdio.h>

int main(void)
{
	int c;

	while ((c = getchar()) != EOF) {
		if (isupper(c))
			c = tolower(c);
		if (putchar(c) == EOF) {
			perror("output error");
			exit(1);
		}
		if (c == '\n')
			fflush(stdout);
	}
	exit(0);
}
