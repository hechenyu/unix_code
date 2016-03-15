#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mysdr.h"

#define	BUFFSIZE	8192	/* buffer size for reads and writes */

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

void
loop(int sockfd, socklen_t salen)
{
	socklen_t		len;
	ssize_t			n;
	char			*p;
	struct sockaddr	*sa;
	struct sap_packet {
	  uint32_t	sap_header;
	  uint32_t	sap_src;
	  char		sap_data[BUFFSIZE];
	} buf;

	if ((sa = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	for ( ; ; ) {
		len = salen;
		if ((n = recvfrom(sockfd, &buf, sizeof(buf) - 1, 0, sa, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		((char *)&buf)[n] = 0;			/* null terminate */
		buf.sap_header = ntohl(buf.sap_header);

		printf("From %s hash 0x%04x\n", sock_ntop(sa, len),
				buf.sap_header & SAP_HASH_MASK);
		if (((buf.sap_header & SAP_VERSION_MASK) >> SAP_VERSION_SHIFT) > 1) {
			fprintf(stderr, "... version field not 1 (0x%08x)\n", buf.sap_header);
			continue;
		}
		if (buf.sap_header & SAP_IPV6) {
			fprintf(stderr, "... IPv6\n");
			continue;
		}
		if (buf.sap_header & (SAP_DELETE|SAP_ENCRYPTED|SAP_COMPRESSED)) {
			fprintf(stderr, "... can't parse this packet type (0x%08x)\n", buf.sap_header);
			continue;
		}
		p = buf.sap_data + ((buf.sap_header & SAP_AUTHLEN_MASK)
							>> SAP_AUTHLEN_SHIFT);
		if (strcmp(p, "application/sdp") == 0)
			p += 16;
		printf("%s\n", p);
	}
}
