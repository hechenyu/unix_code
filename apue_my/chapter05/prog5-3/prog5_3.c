/* 程序5-3 对各个标准I/O流打印缓存状态信息 */
#include <stdio.h>

void pr_stdio(const char *, FILE *);

int main(void)
{
	FILE *fp;

	fputs("enter any character\n", stdout);
	if (getchar() == EOF) {
		perror("getchar error");
		exit(1);
	}
	fputs("one line to standard error\n", stderr);

	pr_stdio("stdin", stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);

	if ((fp = fopen("/etc/motd", "r")) == NULL) {
		perror("fopen error");
		exit(1);
	}
	if (getc(fp) == EOF) {
		perror("getc error");
		exit(1);
	}
	pr_stdio("/etc/motd", fp);
	exit(0);
}

void pr_stdio(const char *name, FILE *fp)
{
	printf("stream = %s, ", name);
							/* following is nonportable */
	if (fp->_flag & _IONBF)
		printf("unbuffered");
	else if (fp->_flag & _IOLBF)
		printf("line buffered");
	else	/* if neither of above */
		printf("fully buffered");
	printf(", buffer size = %d\n", fp->_bufendp - fp->_base);
}
