#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	MAXLINE	4096
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	SEQFILE	"seqno"		/* filename */

void	my_lock(int), my_unlock(int);

int
main(int argc, char **argv)
{
	int		fd;
	long	i, nloop, seqno;
	ssize_t	n;
	char	line[MAXLINE + 1];

	if (argc != 2) {
		fprintf(stderr, "usage: loopmain <loopcount>\n");
		exit(1);
	}
	nloop = atol(argv[1]);

	if ((fd = open(SEQFILE, O_RDWR | O_NONBLOCK, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", SEQFILE, strerror(errno));
		exit(1);
	}

	for (i = 0; i < nloop; i++) {
		my_lock(fd);				/* lock the file */

		if (lseek(fd, 0L, SEEK_SET) == (off_t) -1) {	/* rewind before read */
			perror("lseek error");
			exit(1);
		}
		if ((n = read(fd, line, MAXLINE)) == -1) {
			perror("read error");
			exit(1);
		}
		line[n] = '\0';				/* null terminate for sscanf */

		n = sscanf(line, "%ld\n", &seqno);
		seqno++;					/* increment sequence number */

		snprintf(line, sizeof(line), "%ld\n", seqno);
		if (lseek(fd, 0L, SEEK_SET) == (off_t) -1) {	/* rewind before write */
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
