# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	tcpcli01 tcpcli02 daytimetcpcli tcpcli03 tcpcli04 tcpservselect03 web

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcpcli01: tcpcli01.o strclifork.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli02: tcpcli02.o strclinonb.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli03: tcpcli03.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli04: tcpcli04.o ../lib/str_cli.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli:	daytimetcpcli.o ../lib/connect_nonb.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpservselect03: tcpservselect03.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

web: web.o home_page.o start_connect.o write_get_cmd.o ../lib/tcp_connect.o ../lib/writen.o ../lib/host_serv.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)
