/* Doesn't work right.  Main thread sucks up all the CPU time polling unless
   we call thread_yield(). */
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXLINE     4096    /* max text line length */

#define	min(a, b)	((a) < (b) ? (a) : (b))

#define	MAXFILES	20
#define	SERV		"80"	/* port number or service name */

struct file {
  char	*f_name;			/* filename */
  char	*f_host;			/* hostname or IP address */
  int    f_fd;				/* descriptor */
  int	 f_flags;			/* F_xxx below */
  pthread_t	 f_tid;			/* thread ID */
} file[MAXFILES];
#define	F_CONNECTING	1	/* connect() in progress */
#define	F_READING		2	/* connect() complete; now reading */
#define	F_DONE			4	/* all done */
#define	F_JOINED		8	/* main has pthread_join'ed */

int		nconn, nfiles, nlefttoconn, nlefttoread;
char	get[] = "GET / HTTP/1.0\r\n\r\n";	/* for home page */

int				ndone;		/* number of terminated threads & mutex */
pthread_mutex_t	ndone_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*do_get_read(void *);
void	home_page(const char *, const char *);
void	write_get_cmd(struct file *);

int
tcp_connect(const char *host, const char *serv);

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n);

int
main(int argc, char **argv)
{
	int			i, n, maxnconn;
	pthread_t	tid;
	struct file	*fptr;

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

	nlefttoread = nlefttoconn = nfiles;
	nconn = 0;
	while (nlefttoread > 0) {
/* printf("nconn = %d, nlefttoconn = %d\n", nconn, nlefttoconn); */
		while (nconn < maxnconn && nlefttoconn > 0) {
				/* find a file to read */
			for (i = 0 ; i < nfiles; i++)
				if (file[i].f_flags == 0)
					break;
			if (i == nfiles) {
				fprintf(stderr, "nlefttoconn = %d but nothing found\n", nlefttoconn);
				exit(1);
			}

			if ( (n = pthread_create(&tid, NULL, &do_get_read, &file[i])) != 0) {
				errno = n;
				perror("pthread_create error");
				exit(1);
			}
			printf("created thread %d\n", tid);
			file[i].f_tid = tid;
			file[i].f_flags = F_CONNECTING;
			nconn++;
			nlefttoconn--;
		}
		pthread_yield();

			/* See if one of the threads is done */
		if ( (n = pthread_mutex_lock(&ndone_mutex)) != 0) {
			errno = n;
		   	perror("pthread_mutex_lock error");
			exit(1);
		}
		if (ndone > 0) {
			for (i = 0; i < nfiles; i++) {
				if (file[i].f_flags & F_DONE) {
					if ( (n = pthread_join(file[i].f_tid, (void **) &fptr)) != 0) {
						errno = n;
						perror("pthread_join error");
						exit(1);
					}
		
					if (&file[i] != fptr) {
						fprintf(stderr, "file[i] != fptr\n");
						exit(1);
					}
					fptr->f_flags = F_JOINED;	/* clears F_DONE */
					ndone--;
					nconn--;
					nlefttoread--;
					printf("thread id %d for %s done\n",
							file[i].f_tid, fptr->f_name);
				}
			}
		}
		if ( (n = pthread_mutex_unlock(&ndone_mutex)) != 0) {
			errno = n;
			perror("pthread_mutex_unlock error");
			exit(1);
		}
	}

	exit(0);
}

void *
do_get_read(void *vptr)
{
	int					fd, n;
	char				line[MAXLINE];
	struct file			*fptr;

	fptr = (struct file *) vptr;

	fd = tcp_connect(fptr->f_host, SERV);
	fptr->f_fd = fd;
	printf("do_get_read for %s, fd %d, thread %d\n",
			fptr->f_name, fd, fptr->f_tid);

	write_get_cmd(fptr);	/* write() the GET command */

		/* Read server's reply */
	for ( ; ; ) {
		if ( (n = read(fd, line, MAXLINE)) <= 0) {
			if (n == 0)
				break;		/* server closed connection */
			else {
				perror("read error");
				exit(1);
			}
		}
		printf("read %d bytes from %s\n", n, fptr->f_name);
	}
	printf("end-of-file on %s\n", fptr->f_name);
	close(fd);
	fptr->f_flags = F_DONE;		/* clears F_READING */

	if ( (n = pthread_mutex_lock(&ndone_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_lock error");
		exit(1);
	}
	ndone++;
	if ( (n = pthread_mutex_unlock(&ndone_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_unlock error");
		exit(1);
	}

	return(fptr);		/* terminate thread */
}

void
write_get_cmd(struct file *fptr)
{
	int		n;
	char	line[MAXLINE];

	strcpy(line, "GET ");
	strcat(line, fptr->f_name);
	strcat(line, " HTTP/1.0\r\n\r\n");
	n = strlen(line);
	if (writen(fptr->f_fd, line, n) != n) {
		perror("writen error");
		exit(1);
	}
	printf("wrote %d bytes for %s\n", n, fptr->f_name);

	fptr->f_flags = F_READING;			/* clears F_CONNECTING */
}

void
home_page(const char *host, const char *fname)
{
	int					fd, n;
	char				line[MAXLINE];

	fd = tcp_connect(host, SERV);

	strcpy(line, "GET ");
	strcat(line, fname);
	strcat(line, " HTTP/1.0\r\n\r\n");
	n = strlen(line);
	if (writen(fd, line, n) != n) {
		perror("writen error");
		exit(1);
	}

	for ( ; ; ) {
		if ( (n = read(fd, line, MAXLINE)) <= 0) {
			if (n == 0) {
				break;		/* server closed connection */
			} else {
				perror("read error");
				exit(1);
			}
		}
		printf("read %d bytes of home page\n", n);
		/* do whatever with data */
	}
	printf("end-of-file on home page\n");
	close(fd);
}
