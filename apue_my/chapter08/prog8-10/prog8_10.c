/* 程序8-10 执行一个解释器文件的程序 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}
	else if (pid == 0) {		/* child */
		if (execl("/home1/remit/priv/hexu/unix/prog8-10/testinterp",
					"testinterp", "myarg1", "MY ARG2", (char *) 0) < 0) {
			perror("execl error");
			exit(1);
		}
	}

	if (waitpid(pid, NULL, 0) < 0) {	/* parent */
		perror("waitpid error");
		exit(1);
	}
	exit(0);
}
