# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	daytimetcpsrv2 daytimetcpsrv3

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

daytimetcpsrv2: daytimetcpsrv2.o ../lib/signal.o ../lib/sock_ntop.o tcp_listen.o ../lib/daemon_init.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv3: daytimetcpsrv3.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

