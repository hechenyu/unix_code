/* 程序11-1 禁止中断字符和更改文件结束字符 */
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	struct termios term;
	long vdisable;

	if (isatty(STDIN_FILENO) == 0) {
		perror("standard input is not a terminal device");
		exit(1);
	}

	if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)	{
		perror("fpathconf error or _POSIX_VDISABLE not in effect");
		exit(1);
	}

	if (tcgetattr(STDIN_FILENO, &term) < 0) {		/* fetch tty state */
		perror("tcgetattr error");
		exit(1);
	}

	term.c_cc[VINTR] = vdisable;		/* disable INTR character */
	term.c_cc[VEOF] = 2;		/* EOF is Control-B */

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0) {
		perror("tcsetattr error");
		exit(1);
	}

	exit(0);
}
