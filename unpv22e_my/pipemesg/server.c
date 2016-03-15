#include "mesg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void server(int readfd, int writefd)
{
	FILE *fp;
	ssize_t	n;
	struct mymesg mesg;

	/* 4read pathname from IPC channel */
	mesg.mesg_type = 1;
	if ((n = mesg_recv(readfd, &mesg)) == 0) {
		fprintf(stderr, "pathname missing\n");
		exit(1);
	}
	mesg.mesg_data[n] = '\0';	/* null terminate pathname */

	if ((fp = fopen(mesg.mesg_data, "r")) == NULL) {
		/* 4error: must tell client */
		snprintf(mesg.mesg_data + n, sizeof(mesg.mesg_data) - n,
				 ": can't open, %s\n", strerror(errno));
		mesg.mesg_len = strlen(mesg.mesg_data);
		mesg_send(writefd, &mesg);
	} else {
		/* 4fopen succeeded: copy file to IPC channel */
		while (1) {
			if (fgets(mesg.mesg_data, MAXMESGDATA, fp) == NULL) {
				if (ferror(fp)) {
					perror("fgets error");
					exit(1);
				} else {
					break;
				}
			}
			mesg.mesg_len = strlen(mesg.mesg_data);
			mesg_send(writefd, &mesg);
		}
		if (fclose(fp) != 0) {
			perror("fclose error");
			exit(1);
		}
	}

	/* 4send a 0-length message to signify the end */
	mesg.mesg_len = 0;
	mesg_send(writefd, &mesg);
}
