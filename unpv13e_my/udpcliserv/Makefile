# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	udpcli01 udpcli02 udpcli03 udpcli04 udpcli06 udpcli08 udpcli09 \
	udpcli10 \
	udpserv01 udpserv06 udpserv07 \
	udpservselect01 

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

udpcli01: udpcli01.o ../lib/dg_cli.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli02: udpcli02.o dgcliaddr.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli03: udpcli03.o dgcliinetaddr.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli04: udpcli04.o dgcliconnect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli06: udpcli06.o dgcliloop1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli08: udpcli08.o dgcliloop3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli09: udpcli09.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpcli10: udpcli10.o dgclibig.o ../lib/udp_client.o 
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv01: udpserv01.o ../lib/dg_echo.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv06: udpserv06.o dgecholoop1.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv07: udpserv07.o dgecholoop2.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpservselect01: udpservselect01.o sigchldwaitpid.o ../lib/signal.o ../lib/str_echo.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)


