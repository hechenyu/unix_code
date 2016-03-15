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
	int fd;
	long i, seqno;
	pid_t pid;
	ssize_t	n;
	char line[MAXLINE + 1];

	pid = getpid();
	if ((fd = open(SEQFILE, O_RDWR, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", SEQFILE, strerror(errno));
		exit(1);
	}

	for (i = 0; i < 20; i++) {
		my_lock(fd);				/* lock the file */

		if (lseek(fd, 0L, SEEK_SET) == (off_t) -1) {	/* rewind before read */
			printf("lseek error: %s\n", strerror(errno));
			exit(1);
		}
		if ((n = read(fd, line, MAXLINE)) < 0) {
			printf("read error: %s\n", strerror(errno));
			exit(1);
		}
		line[n] = '\0';				/* null terminate for sscanf */

		n = sscanf(line, "%ld\n", &seqno);
		printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long) pid, seqno);

		seqno++;					/* increment sequence number */

		snprintf(line, sizeof(line), "%ld\n", seqno);
		if (lseek(fd, 0L, SEEK_SET) == (off_t) -1) {	/* rewind before read */
			printf("lseek error: %s\n", strerror(errno));
			exit(1);
		}
		if (write(fd, line, strlen(line)) != strlen(line)) {
			printf("write error: %s\n", strerror(errno));
			exit(1);
		}

		my_unlock(fd);				/* unlock the file */
	}
	exit(0);
}
