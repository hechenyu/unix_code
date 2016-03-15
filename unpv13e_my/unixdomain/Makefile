# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	unixbind unixstrserv01 unixstrcli01 unixdgserv01 unixdgcli01 \
		mycat openfile daytimetcpcli daytimetcpsrv2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

unixbind: unixbind.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

unixstrserv01: unixstrserv01.o sigchldwaitpid.o ../lib/str_echo.o ../lib/readn.o ../lib/writen.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

unixstrcli01: unixstrcli01.o ../lib/str_cli.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

unixdgserv01: unixdgserv01.o ../lib/dg_echo.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

unixdgcli01: unixdgcli01.o ../lib/dg_cli.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mycat: mycat.o myopen.o ../lib/read_fd.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

openfile: openfile.o ../lib/write_fd.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli: daytimetcpcli.o ../lib/tcp_connect.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv2: daytimetcpsrv2.o ../lib/tcp_listen.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

