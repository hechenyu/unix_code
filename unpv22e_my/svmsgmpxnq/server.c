#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define	MAXLINE	4096

#include	"mesg.h"

void
server(int readid, int writeid)
{
	FILE	*fp;
	char	*ptr;
	ssize_t	n;
	struct mymesg	mesg;
	void	sig_chld(int);
	pid_t childpid;

	signal(SIGCHLD, sig_chld);

	for ( ; ; ) {
			/* 4read pathname from our well-known queue */
		mesg.mesg_type = 1;
		if ((n = mesg_recv(readid, &mesg)) == -1) {
			perror("mesg_recv error");
			exit(1);
		} else if (n == 0) {
			fprintf(stderr, "pathname missing\n");
			continue;
		}
		mesg.mesg_data[n] = '\0';	/* null terminate pathname */

		if ((ptr = strchr(mesg.mesg_data, ' ')) == NULL) {
			fprintf(stderr, "bogus request: %s", mesg.mesg_data);
			continue;
		}
		*ptr++ = 0;			/* null terminate msgid, ptr = pathname */
		writeid = atoi(mesg.mesg_data);

		childpid = fork();
		if (childpid == -1) {
			perror("fork error");
			exit(1);
		}
		if (childpid == 0) {		/* child */
			if ((fp = fopen(ptr, "r")) == NULL) {
					/* 4error: must tell client */
				snprintf(mesg.mesg_data + n, sizeof(mesg.mesg_data) - n,
						 ": can't open, %s\n", strerror(errno));
				mesg.mesg_len = strlen(ptr);
				memmove(mesg.mesg_data, ptr, mesg.mesg_len);
				if (mesg_send(writeid, &mesg) == -1) {
					perror("mesg_send error");
					exit(1);
				}
		
			} else {
					/* 4fopen succeeded: copy file to client's queue */
				while (fgets(mesg.mesg_data, MAXMESGDATA, fp) != NULL) {
					mesg.mesg_len = strlen(mesg.mesg_data);
					if (mesg_send(writeid, &mesg) == -1) {
						perror("mesg_send error");
						exit(1);
					}
				}
				if (ferror(fp)) {
					perror("fgets error");
					exit(1);
				}
				if (fclose(fp) != 0) {
					perror("fclose error");
					exit(1);
				}
			}
		
				/* 4send a 0-length message to signify the end */
			mesg.mesg_len = 0;
			if (mesg_send(writeid, &mesg) == -1) {
				perror("mesg_send error");
				exit(1);
			}
			exit(0);		/* child terminates */
		}
		/* parent just loops around */
	}
}
