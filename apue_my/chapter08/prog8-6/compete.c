/* 程序8-6 具有竞态条件的程序 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

static void charatatime(char *);

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {
		charatatime("output from child\n");
	} else {
		charatatime("output from parent\n");
	}
	exit(0);
}

static void charatatime(char *str)
{
	char *ptr;
	int c;

	setbuf(stdout, NULL);		/* set unbuffered */
	for (ptr = str; c = *ptr++; )
		putc(c, stdout);
}

