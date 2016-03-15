# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

# XXX get autoconf to put tcprecv03p in here if the system supports poll
PROGS =	tcprecv01 tcprecv02 tcprecv03 tcprecv04 tcprecv05 tcprecv06 \
	 tcpsend01 tcpsend02 tcpsend03 tcpsend04 tcpsend05 tcpsend06 \
	 tcpcli02 tcpserv02

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcprecv01:	tcprecv01.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpsend01:	tcpsend01.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcprecv02:	tcprecv02.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpsend02:	tcpsend02.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcprecv03:	tcprecv03.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcprecv03p:	tcprecv03p.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpsend03:	tcpsend03.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcprecv04:	tcprecv04.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpsend04:	tcpsend04.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcprecv05:	tcprecv05.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpsend05:	tcpsend05.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcprecv06:	tcprecv06.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpsend06:	tcpsend06.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli02:	tcpcli02.o strcliselect02.o heartbeatcli.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv02:	tcpserv02.o strecho02.o heartbeatserv.o sigchldwaitpid.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

