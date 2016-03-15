/* 程序10-6 在一个捕捉其他信号的程序中调用sleep2 */
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

unsigned int sleep2(unsigned int);
static void sig_int(int);

int main(void)
{
	unsigned int unslept;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal(SIGINT) error");
		exit(1);
	}

	unslept = sleep2(5);
	printf("sleep2 returned: %u\n", unslept);

	exit(0);
}

static void sig_int(int signo)
{
	int i;
	volatile int j;

	printf("\nsig_int starting\n");
	for (i = 0; i < 2000000; i++)
		j += i * i;
	printf("sig_int finished\n");
	return;
}
