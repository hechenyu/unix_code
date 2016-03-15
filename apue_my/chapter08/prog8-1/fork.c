/* ³ÌÐò8-1 forkº¯ÊýÊµÀý */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int glob = 6;		/* external variable in initialized data */
char buf[] = "a write to stdout\n";

int main(void)
{
	int var;		/* automatic variable on the stack */
	pid_t pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) {
		perror("write error");
		exit(1);
	}
	printf("before fork1\n");	/* we don't flush stdout */

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}
	else if (pid == 0) {		/* child */
		glob++;					/* modify variables */
		var++;
	} else
		sleep(2);				/* parent */

	printf("1)pid = %d, glob = %d, var = %d\n", getpid(), glob, var);		

	printf("before fork2\n");	/* we don't flush stdout */

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}
	else if (pid == 0) {		/* child */
		glob++;					/* modify variables */
		var++;
	} else
		sleep(2);				/* parent */

	printf("2)pid = %d, glob = %d, var = %d\n", getpid(), glob, var);		

	exit(0);
}
