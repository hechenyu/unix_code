# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	tcpcli01 tcpserv01

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcpcli01:	tcpcli01.o sourceroute.o ../lib/host_serv.o ../lib/str_cli.o ../lib/writen.o ../lib/readline.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv01:	tcpserv01.o sourceroute.o sigchldwaitpid.o ../lib/host_serv.o ../lib/str_echo.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli01:	udpcli01.o ../lib/host_serv.o ../lib/dg_cli.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv01:	udpserv01.o sourceroute6.o dgechoprintroute.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

