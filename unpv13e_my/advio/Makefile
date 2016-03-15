# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	tcpserv02 tcpcli01 tcpcli02 udpcli01 udpcli02 udpcli03 \
	udpserv01 udpserv03 udpserv04

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcpserv02: tcpserv02.o sig_chld_waitpid.o str_echo_stdio02.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli01: tcpcli01.o ../lib/str_cli.o ../lib/readline.o ../lib/writen.o ../lib/connect_timeo.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli02: tcpcli02.o str_cli_select02.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli01: udpcli01.o dgclitimeo1.o ../lib/readable_timeo.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli02: udpcli02.o dgclitimeo2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli03: udpcli03.o dgclitimeo3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv01: udpserv01.o dgechoaddr.o recvfromflags.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv03: udpserv03.o ../lib/sock_ntop.o ../lib/get_ifi_info.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv04: udpserv04.o ../lib/sock_ntop.o ../lib/get_ifi_info.o ../lib/udp_client.o ../lib/sock_set_port.o ../lib/sock_get_port.o ../lib/sock_set_addr.o ../lib/sock_set_wild.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

