# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	udpcli01

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

udpcli01:	udpcli01.o dg_cli.o dg_send_recv.o ../lib/rtt.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

