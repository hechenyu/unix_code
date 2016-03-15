# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

OBJS = icmpd.o readable_listen.o readable_conn.o readable_v4.o readable_v6.o

PROGS =	icmpd udpcli01

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

icmpd:	${OBJS} ../lib/read_fd.o ../lib/sock_get_port.o ../lib/sock_ntop_host.o ../lib/sock_bind_wild.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli01:	udpcli01.o dgcli01.o ../lib/sock_ntop.o ../lib/udp_client.o ../lib/sock_bind_wild.o ../lib/write_fd.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

