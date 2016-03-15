/* 程序11-12 打印窗口大小 */
#include <signal.h>
#include <termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>		/* 4.3+BSD requires this too */
#endif
#include <unistd.h>
#include <stdio.h>

static void pr_winsize(int), sig_winch(int);

int main(void)
{
	if (isatty(STDIN_FILENO) == 0)
		exit(1);

	if (signal(SIGWINCH, sig_winch) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	pr_winsize(STDIN_FILENO);		/* print initial size */
	for ( ; ; )
		pause();
}

static void pr_winsize(int fd)
{
	struct winsize size;

	if (ioctl(fd, TIOCGWINSZ, (char *) &size) < 0) {
		perror("TIOCGWINSZ error");
		exit(1);
	}
	printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo)
{
	printf("SIGWINCH received\n");
	pr_winsize(STDIN_FILENO);
	return;
}
