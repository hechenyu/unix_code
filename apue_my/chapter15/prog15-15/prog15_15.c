/* ³ÌÐò15-15 mainº¯Êý */
#inlcude "opend.h"

			/* define global variables */
char errmsg[MAXLINE];
int oflag;
char *pathname;

int main(void)
{
	int nread;
	char buf[MAXLINE];

	for ( ; ; ) {		/* read arg buffer from client, process request */
		if ((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0) {
			perror("read error on stream pipe\n");
			exit(1);
		} else if (nread == 0)
			break;		/* client has closed the stream pipe */

		requrest(buf, nread, STDOUT_FILENO);
	}
	exit(0);
}

