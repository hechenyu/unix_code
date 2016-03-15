/* ³ÌÐò8-8 execº¯ÊýÊµÀý */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {		/* specify pathname, specify environment */
		if (execle("/home1/remit/priv/hexu/unix/prog8-9/echoall",
					"echoall", "myarg1", "MY ARG2", (char *) 0,
					env_init) < 0) {
			perror("execle error");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0) {
		perror("wait error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {		/* specify filename, inherit environment */
		if (execlp("echoall",
					"echoall", "only 1 arg", (char *) 0) < 0) {
			perror("execlp error");
			exit(1);
		}
	}
	exit(0);
}

