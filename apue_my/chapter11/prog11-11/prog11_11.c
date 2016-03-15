/* 程序11-11 测试原始和cbreak工作方式 */
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_catch(int);
int tty_cbreak(int);
int tty_raw(int);
int tty_reset(int);
void tty_atexit(void);
struct termios *tty_termios(void);

int main(void)
{
	int i;
	char c;

	if (signal(SIGINT, sig_catch) == SIG_ERR) {		/* catch signals */
		perror("signal(SIGINT) error");
		exit(1);
	}
	if (signal(SIGQUIT, sig_catch) == SIG_ERR) {
		perror("signal(SIGQUIT) error");
		exit(1);
	}
	if (signal(SIGTERM, sig_catch) == SIG_ERR) {
		perror("signal(SIGTERM) error");
		exit(1);
	}

	if (tty_raw(STDIN_FILENO) < 0) {
		perror("tty_raw error");
		exit(1);
	}
	printf("Enter raw mode characers, terminate with DELETE\n");
	while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
		if ((c &= 255) == 0177)		/* 0177 = ASCII DELETE */
			break;
		printf("%o\n", c);
	}
	if (tty_reset(STDIN_FILENO) < 0) {
		perror("tty_reset error");
		exit(1);
	}
	if (i <= 0) {
		perror("read error");
		exit(1);
	}
	if (tty_cbreak(STDIN_FILENO) < 0) {
		perror("tty_raw error");
		exit(1);
	}
	printf("\nEnter cbreak mod characters, terminate with SIGINT\n");
	while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
		c &= 255;
		printf("%0\n", c);
	}
	tty_reset(STDIN_FILENO);
	if (i <= 0) {
		perror("read error");
		exit(1);
	}
	exit(0);
}

static void sig_catch(int signo)
{
	printf("signal caught\n");
	tty_reset(STDIN_FILENO);
	exit(0);
}
	
