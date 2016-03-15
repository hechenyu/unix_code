# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE
INCLUDE = 
LDFLAGS = -lpthread
LDPATH = 

PROGS =	web01 web02 web03 \
	tcpcli01 tcpcli02 tcpserv01 tcpserv02 \
	test01 example01 example02 example03


.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

web01:	web01.o ../lib/tcp_connect.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

web02:	web02.o ../lib/tcp_connect.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

web03:	web03.o ../lib/tcp_connect.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli01:	tcpcli01.o strclithread.o ../lib/tcp_connect.o ../lib/writen.o ../lib/readline.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli02:	tcpcli02.o strclithread2.o ../lib/tcp_connect.o ../lib/writen.o ../lib/readline.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpserv01:	tcpserv01.o ../lib/str_echo.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Broken one that uses readline() from library.
tcpserv02:	tcpserv02.o ../lib/str_echo.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Correct one that uses thread-safe readline().
tcpserv02g:	tcpserv02.o readline.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test01:	test01.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test02:	test02.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test03:	test03.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Bad version uses readline() from library.
test04b:	test04.o ../lib/readline.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Good version uses readline.c in this directory.
test04g:	test04.o readline.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test05:	test05.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

example01:	example01.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

example02:	example02.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

example03:	example03.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

