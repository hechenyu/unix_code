/* 程序8-3 打印exit状态的说明 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void pr_exit(int status);

int main(void)
{
	pid_t pid;
	int status;

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}
	else if (pid == 0)		/* child */
		exit(7);
	
	if (wait(&status) != pid) {		/* wait for child */
		perror("wait error");
		exit(1);
	}
	pr_exit(status);		/* and print its status */

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0)	/* child */
		abort();			/* generates SIGABRT */

	if (wait(&status) != pid) {		/* wait for child */
		perror("wait error");
		exit(1);
	}
	pr_exit(status);		/* and print its status */

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0)	/* child */
		status /= 0;		/* divide by 0 generates SIGFPE */

	if (wait(&status) != pid) {		/* wait for child */
		perror("wait error");
		exit(1);
	}
	pr_exit(status);		/* and print its status */

	exit(0);
}
		
void pr_exit(int status)
{
	if (WIFEXITED(status))
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef	WCOREDUMP
				WCOREDUMP(status) ? " (core file generated)" : "");
#else
				"");
#endif
	else if (WIFSTOPPED(status))
		printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}
