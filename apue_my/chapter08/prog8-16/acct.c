/* 程序8-16 产生会计数据的程序 */
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid != 0) {	/* parent */
		sleep(2);
		exit(2);		/* terminate with exit status 2 */
	}

							/* first child */
	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid != 0) {
		sleep(4);
		abort();		/* terminate with core dump */
	}

							/* second child */
	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid != 0) {
		execl("/usr/bin/dd", "dd", "if=/boot", "of=/dev/null", NULL);
		exit(7);			/* shouldn't get here */
	}

							/* third child */
	if ((pid = fork()) < 0) {	
		perror("fork() error");
		exit(1);
	} else if (pid != 0) {
		sleep(8);
		exit(0);		/* normal exit */
	}

							/* fourth child */
	sleep(6);
	kill(getpid(), SIGKILL);	/* terminate with signal, no core dump */
	exit(6);					/* shouldn't get here */
}
