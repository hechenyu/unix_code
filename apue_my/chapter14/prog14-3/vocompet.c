/* 程序8-7 修改程序8-6以避免竞态条件 */
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void charatatime(char *);
static void sig_usr(int);
void TELL_WAIT(void);
void TELL_PARENT(pid_t);
void WAIT_PARENT(void);
void TELL_CHILD(pid_t);
void WAIT_CHILD(void);

int main(void)
{
	pid_t pid;

	TELL_WAIT();

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {
		WAIT_PARENT();		/* parent goes first */
		charatatime("output from child\n");
	} else {
		charatatime("output from parent\n");
		TELL_CHILD(pid);
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

