/* 程序10-19 用system调用ed编辑程序 */
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int my_system(const char *);
static void sig_int(int), sig_chld(int);

int main(void)
{
	int status;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal(SIGINT) error");
		exit(1);
	}
	if (signal(SIGCHLD, sig_chld) == SIG_ERR) {
		perror("signal(SIGCHLD) error");
		exit(1);
	}

	if ((status = my_system("/bin/ed")) < 0) {
		perror("my_system() error");
		exit(1); 
	}

	exit(0);
}

static void sig_int(int signo)
{
	printf("caught SIGINT\n");
	return;
}

static void sig_chld(int signo)
{
	printf("caught SIGCHLD\n");
	return;
}

