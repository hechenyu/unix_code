/* test readline() */
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static char	*infile;	/* from argv[1]; read-only by threads */

#define MAXLINE     4096    /* max text line length */

ssize_t
readline(int fd, void *vptr, size_t maxlen);

void *
myfunc(void *ptr)
{
	int		i, fdin;
	char	buf[MAXLINE];
	FILE	*fpout;
	int		n;

	snprintf(buf, sizeof(buf), "temp.%d", pthread_self());
	if ((fpout = fopen(buf, "w+")) == NULL) {
		perror("fopen error");
		exit(1);
	}
	/* printf("created %s\n", buf); */

	for (i = 0; i < 5; i++) {
		if ((fdin = open(infile, O_RDONLY, 0)) == -1) {
			fprintf(stderr, "open error for %s: %s\n", infile, strerror(errno));
			exit(1);
		}

		while ((n = readline(fdin, buf, sizeof(buf))) > 0) {
			fputs(buf, fpout);
		}
		if (n == -1) {
			perror("readline error");
			exit(1);
		}
		if (close(fdin) == -1) {
			perror("close error");
			exit(1);
		}
	}
	if (fclose(fpout) != 0) {
		perror("fclose error");
		exit(1);
	}

	printf("thread %d done\n", pthread_self());
	return(NULL);
}

int
main(int argc, char **argv)
{
	int				i, nthreads;
	pthread_t		tid;
	int				n;

	if (argc != 3) {
		fprintf(stderr, "usage: test04 <input-file> <#threads>\n");
		exit(1);
	}
	infile = argv[1];
	nthreads = atoi(argv[2]);

	for (i = 0; i < nthreads; i++) {
		if ((n = pthread_create(&tid, NULL, myfunc, NULL)) != 0) {
			errno = n;
			perror("pthread_create error");
			exit(1);
		}
	}

	pause();

	exit(0);
}
