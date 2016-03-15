/* include web1 */
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "web.h"

#define MAXLINE     4096    /* max text line length */

#define	min(a, b)	((a) < (b) ? (a) : (b))

int
main(int argc, char **argv)
{
	int		i, fd, n, maxnconn, flags, error;
	char	buf[MAXLINE];
	fd_set	rs, ws;

	if (argc < 5) {
		fprintf(stderr, "usage: web <#conns> <hostname> <homepage> <file1> ...\n");
		exit(1);
	}
	maxnconn = atoi(argv[1]);

	nfiles = min(argc - 4, MAXFILES);
	for (i = 0; i < nfiles; i++) {
		file[i].f_name = argv[i + 4];
		file[i].f_host = argv[2];
		file[i].f_flags = 0;
	}
	printf("nfiles = %d\n", nfiles);

	home_page(argv[2], argv[3]);

	FD_ZERO(&rset);
	FD_ZERO(&wset);
	maxfd = -1;
	nlefttoread = nlefttoconn = nfiles;
	nconn = 0;
/* end web1 */
/* include web2 */
	while (nlefttoread > 0) {
		while (nconn < maxnconn && nlefttoconn > 0) {
				/* 4find a file to read */
			for (i = 0 ; i < nfiles; i++)
				if (file[i].f_flags == 0)
					break;
			if (i == nfiles) {
				fprintf(stderr, "nlefttoconn = %d but nothing found\n", nlefttoconn);
				exit(1);
			}
			start_connect(&file[i]);
			nconn++;
			nlefttoconn--;
		}

		rs = rset;
		ws = wset;
		if ((n = select(maxfd+1, &rs, &ws, NULL, NULL)) < 0) {
			perror("select error");
			exit(1);
		}

		for (i = 0; i < nfiles; i++) {
			flags = file[i].f_flags;
			if (flags == 0 || flags & F_DONE)
				continue;
			fd = file[i].f_fd;
			if (flags & F_CONNECTING &&
				(FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {
				n = sizeof(error);
				if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &n) < 0 ||
					error != 0) {
					fprintf(stderr, "nonblocking connect failed for %s: %s\n",
							file[i].f_name, strerror(errno));
				}
					/* 4connection established */
				printf("connection established for %s\n", file[i].f_name);
				FD_CLR(fd, &wset);		/* no more writeability test */
				write_get_cmd(&file[i]);/* write() the GET command */

			} else if (flags & F_READING && FD_ISSET(fd, &rs)) {
				if ( (n = read(fd, buf, sizeof(buf))) < 0) {
					perror("read error");
					exit(1);
				} else if (n == 0) {
					printf("end-of-file on %s\n", file[i].f_name);
					if (close(fd) == -1) {
						perror("close error");
						exit(1);
					}
					file[i].f_flags = F_DONE;	/* clears F_READING */
					FD_CLR(fd, &rset);
					nconn--;
					nlefttoread--;
				} else {
					printf("read %d bytes from %s\n", n, file[i].f_name);
				}
			}
		}
	}
	exit(0);
}
/* end web2 */
