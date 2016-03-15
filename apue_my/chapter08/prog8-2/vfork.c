/* ³ÌÐò8-2 vforkº¯ÊýÊµÀý */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int glob = 6;		/* external variable in initialized data */

int main(void)
{
	int var;		/* automatic variable on the stack */
	pid_t pid;

	var = 88;
	printf("before vfork\n");	/* we don't flush stdout */

	if ((pid = vfork()) < 0) {
		perror("fork error");
		exit(1);
	}
	else if (pid == 0) {		/* child */
		glob++;					/* modify variables */
		var++;
		_exit(0);				/* child terminates */
	} 

	/* parent */
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);		
	exit(0);
}
