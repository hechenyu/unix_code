/* ³ÌÐò11-2 tcgetattrÊµÀý */
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	struct termios term;
	int size;

	if (tcgetattr(STDIN_FILENO, &term) < 0) {
		perror("tcgetattr  error");
		exit(0);
	}

	size = term.c_cflag & CSIZE;
	if (size == CS5)
		printf("5 bits/byte\n");
	else if (size == CS6)
		printf("6 bits/byte\n");
	else if (size == CS7)
		printf("7 bits/byte\n");
	else
		printf("unknown bits/byte\n");

	term.c_cflag &= ~CSIZE;			/* zero out the bits */
	term.c_cflag |= CS8;			/* set 8 bits/byte */

	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0) {
		printf("tcsetattr error");
		exit(1);
	}

	exit(0);
}
