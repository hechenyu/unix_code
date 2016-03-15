#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	MAXLINE	4096
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	SEQFILE	"seqno"		/* filename */

void my_lock(int), my_unlock(int);

int main(int argc, char **argv)
{
	int fd, i, seqno, c;
	pid_t pid;
	ssize_t	n;
	char line[MAXLINE + 1], *ptr;

	pid = getpid();
	if ((fd = open(SEQFILE, O_RDWR | O_CREAT, FILE_MODE)) == -1) {
		printf("open error for %s: %s\n", SEQFILE, strerror(errno));
		exit(1);
	}

	setvbuf(stdout, NULL, _IONBF, 0);	/* unbuffered */

	for (i = 0; i < 20; i++) {
		my_lock(fd);				/* lock the file */

		if (lseek(fd, 0L, SEEK_SET) == (off_t) -1) {	/* rewind before read */
			perror("lseek error");
			exit(1);
		}
		if ((n = read(fd, line, MAXLINE)) < 0) {
			perror("read error");
			exit(1);
		}
		line[n] = '\0';				/* null terminate for sscanf */

		n = sscanf(line, "%d\n", &seqno);
		snprintf(line, sizeof(line), "pid = %d, seq# = %d\n", pid, seqno);
		for (ptr = line; (c = *ptr++) != 0; )
			putchar(c);

		seqno++;					/* increment sequence number */

		sprintf(line, "%03d\n", seqno);
		if (lseek(fd, 0, SEEK_SET) == (off_t) -1) {		/* rewind before write */
			perror("lseek error");
			exit(1);
		}
		if (write(fd, line, strlen(line)) != strlen(line)) {
			perror("write error");
			exit(1);
		}

		my_unlock(fd);				/* unlock the file */
	}
	exit(0);
}
