/* include nonb1 */
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define VOL2 1

#define MAXLINE     4096    /* max text line length */

#define max(a,b)    ((a) > (b) ? (a) : (b))

char *
gf_time(void);

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, val, stdineof;
	ssize_t		n, nwritten;
	fd_set		rset, wset;
	char		to[MAXLINE], fr[MAXLINE];
	char		*toiptr, *tooptr, *friptr, *froptr;

	if ((val = fcntl(sockfd, F_GETFL, 0)) == -1) {
		perror("fcntl error");
		exit(1);
	}
	if (fcntl(sockfd, F_SETFL, val | O_NONBLOCK) == -1) {
		perror("fcntl error");
		exit(1);
	}

	if ((val = fcntl(STDIN_FILENO, F_GETFL, 0)) == -1) {
		perror("fcntl error");
		exit(1);
	}	
	if (fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK) == -1) {
		perror("fcntl error");
		exit(1);
	}

	if ((val = fcntl(STDOUT_FILENO, F_GETFL, 0)) == -1) {
		perror("fcntl error");
		exit(1);
	}
	if (fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK) == -1) {
		perror("fcntl error");
		exit(1);
	}

	toiptr = tooptr = to;	/* initialize buffer pointers */
	friptr = froptr = fr;
	stdineof = 0;

	maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
	for ( ; ; ) {
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if (stdineof == 0 && toiptr < &to[MAXLINE])
			FD_SET(STDIN_FILENO, &rset);	/* read from stdin */
		if (friptr < &fr[MAXLINE])
			FD_SET(sockfd, &rset);			/* read from socket */
		if (tooptr != toiptr)
			FD_SET(sockfd, &wset);			/* data to write to socket */
		if (froptr != friptr)
			FD_SET(STDOUT_FILENO, &wset);	/* data to write to stdout */

		if (select(maxfdp1, &rset, &wset, NULL, NULL) < 0) {
			perror("select error");
			exit(1);
		}
/* end nonb1 */
/* include nonb2 */
		if (FD_ISSET(STDIN_FILENO, &rset)) {
			if ( (n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0) {
				if (errno != EWOULDBLOCK) {
					perror("read error on stdin");
					exit(1);
				}	

			} else if (n == 0) {
#ifdef	VOL2
				fprintf(stderr, "%s: EOF on stdin\n", gf_time());
#endif
				stdineof = 1;			/* all done with stdin */
				if (tooptr == toiptr) {
					if (shutdown(sockfd, SHUT_WR) < 0) {/* send FIN */
						perror("shutdown error");
						exit(1);
					}
				}

			} else {
#ifdef	VOL2
				fprintf(stderr, "%s: read %d bytes from stdin\n", gf_time(), n);
#endif
				toiptr += n;			/* # just read */
				FD_SET(sockfd, &wset);	/* try and write to socket below */
			}
		}

		if (FD_ISSET(sockfd, &rset)) {
			if ( (n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
				if (errno != EWOULDBLOCK) {
					perror("read error on socket");
					exit(1);
				}

			} else if (n == 0) {
#ifdef	VOL2
				fprintf(stderr, "%s: EOF on socket\n", gf_time());
#endif
				if (stdineof)
					return;		/* normal termination */
				else {
					fprintf(stderr, "str_cli: server terminated prematurely\n");
					exit(1);
				}

			} else {
#ifdef	VOL2
				fprintf(stderr, "%s: read %d bytes from socket\n",
								gf_time(), n);
#endif
				friptr += n;		/* # just read */
				FD_SET(STDOUT_FILENO, &wset);	/* try and write below */
			}
		}
/* end nonb2 */
/* include nonb3 */
		if (FD_ISSET(STDOUT_FILENO, &wset) && ( (n = friptr - froptr) > 0)) {
			if ( (nwritten = write(STDOUT_FILENO, froptr, n)) < 0) {
				if (errno != EWOULDBLOCK) {
					perror("write error to stdout");
					exit(1);
				}

			} else {
#ifdef	VOL2
				fprintf(stderr, "%s: wrote %d bytes to stdout\n",
								gf_time(), nwritten);
#endif
				froptr += nwritten;		/* # just written */
				if (froptr == friptr)
					froptr = friptr = fr;	/* back to beginning of buffer */
			}
		}

		if (FD_ISSET(sockfd, &wset) && ( (n = toiptr - tooptr) > 0)) {
			if ( (nwritten = write(sockfd, tooptr, n)) < 0) {
				if (errno != EWOULDBLOCK) {
					perror("write error to socket");
					exit(1);
				}

			} else {
#ifdef	VOL2
				fprintf(stderr, "%s: wrote %d bytes to socket\n",
								gf_time(), nwritten);
#endif
				tooptr += nwritten;	/* # just written */
				if (tooptr == toiptr) {
					toiptr = tooptr = to;	/* back to beginning of buffer */
					if (stdineof) {
						if (shutdown(sockfd, SHUT_WR) < 0) {	/* send FIN */
							perror("shutdown error");
							exit(1);
						}
					}
				}
			}
		}
	}
}
/* end nonb3 */
