# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	tcpcli01 tcpcli04 tcpcli05 tcpcli06 \
	tcpcli07 tcpcli08 tcpcli09 tcpcli11 \
	tcpserv01 tcpserv02 tcpserv03 tcpserv04 \
	tcpserv08 tcpserv09 tcpservselect01 tcpservpoll01 tsigpipe

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcpcli01: tcpcli01.o ../lib/str_cli.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli04: tcpcli04.o ../lib/str_cli.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli05: tcpcli05.o ../lib/str_cli.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli06: tcpcli06.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli07: tcpcli07.o ../lib/str_cli.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli08: tcpcli08.o str_cli08.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli09: tcpcli09.o str_cli09.o ../lib/readn.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli11: tcpcli11.o str_cli11.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv01: tcpserv01.o ../lib/str_echo.o ../lib/readn.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv02: tcpserv02.o sigchldwait.o ../lib/str_echo.o ../lib/readn.o ../lib/writen.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv03: tcpserv03.o sigchldwait.o ../lib/str_echo.o ../lib/readn.o ../lib/writen.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv04: tcpserv04.o sigchldwaitpid.o ../lib/str_echo.o ../lib/readn.o ../lib/writen.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv08: tcpserv08.o sigchldwaitpid.o str_echo08.o ../lib/readline.o ../lib/writen.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv09: tcpserv09.o sigchldwaitpid.o str_echo09.o ../lib/readn.o ../lib/writen.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpservselect01: tcpservselect01.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpservpoll01: tcpservpoll01.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tsigpipe: tsigpipe.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

