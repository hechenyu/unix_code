/* 程序14-3 使父、子进程同步的例程 */
#include <unistd.h>
#include <stdio.h>

static int pfd1[2], pfd2[2];

void TELL_WAIT(void)
{
	if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
		perror("pipe error");
		exit(1);
	}
}

void TELL_PARENT(pid_t pid)
{
	if (write(pfd2[1], "c", 1) != 1) {
		perror("write error");
		exit(1);
	}
}

void WAIT_PARENT(void)
{
	char c;

	if (read(pfd1[0], &c, 1) != 1) {
		perror("read error");
		exit(1);
	}
	if (c != 'p') {
		perror("WAIT_PARENT: incorrect data");
		exit(1);
	}
}

void TELL_CHILD(pid_t pid)
{
	if (write(pfd1[1], "p", 1) != 1) {
		perror("write error");
		exit(1);
	}
}

void WAIT_CHILD(void)
{
	char c;

	if (read(pfd2[0], &c, 1) != 1) {
		perror("read error");
		exit(1);
	}
	if (c != 'c') {
		perror("WAIT_CHILD: incorrect data");
		exit(1);
	}
}
