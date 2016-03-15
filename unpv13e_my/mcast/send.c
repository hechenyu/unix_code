#include <sys/socket.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	SENDRATE	5		/* send one datagram every five seconds */

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

void
send_all(int sendfd, SA *sadest, socklen_t salen)
{
	char		line[MAXLINE];		/* hostname and process ID */
	struct utsname	myname;
	size_t len;

	if (uname(&myname) < 0) {
		perror("uname error");;
		exit(1);
	}
	snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());

	for ( ; ; ) {
		len = strlen(line);
		if (sendto(sendfd, line, len, 0, sadest, salen) != len) {
			perror("sendto error");
			exit(1);
		}

		sleep(SENDRATE);
	}
}
