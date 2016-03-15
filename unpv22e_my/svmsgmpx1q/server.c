#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	MAXLINE	4096

#include	"mesg.h"

void
server(int readfd, int writefd)
{
	FILE	*fp;
	char	*ptr;
	pid_t	pid;
	ssize_t	n;
	struct mymesg	mesg;

	for ( ; ; ) {
			/* 4read pathname from IPC channel */
		mesg.mesg_type = 1;
		if ((n = mesg_recv(readfd, &mesg)) == -1) {
			perror("mesg_recv error");
			exit(1);
		} else if (n == 0) {
			fprintf(stderr, "pathname missing\n");
			continue;
		}
		mesg.mesg_data[n] = '\0';	/* null terminate pathname */

		if ( (ptr = strchr(mesg.mesg_data, ' ')) == NULL) {
			fprintf(stderr, "bogus request: %s\n", mesg.mesg_data);
			continue;
		}

		*ptr++ = 0;			/* null terminate PID, ptr = pathname */
		pid = atol(mesg.mesg_data);
		mesg.mesg_type = pid;	/* for messages back to client */

		if ( (fp = fopen(ptr, "r")) == NULL) {
				/* 4error: must tell client */
			snprintf(mesg.mesg_data + n, sizeof(mesg.mesg_data) - n,
					 ": can't open, %s\n", strerror(errno));
			mesg.mesg_len = strlen(ptr);
			memmove(mesg.mesg_data, ptr, mesg.mesg_len);
			if (mesg_send(writefd, &mesg) == -1) {
				perror("mesg_send error");
				exit(1);
			}
	
		} else {
				/* 4fopen succeeded: copy file to IPC channel */
			while (fgets(mesg.mesg_data, MAXMESGDATA, fp) != NULL) {
				mesg.mesg_len = strlen(mesg.mesg_data);
				if (mesg_send(writefd, &mesg) == -1) {
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
		if (mesg_send(writefd, &mesg) == -1) {
			perror("mesg_send error");
			exit(1);
		}
	}
}
