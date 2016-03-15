# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	udpcli01 udpserv01

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

udpcli01:	udpcli01.o dgcli01.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv01:	udpserv01.o dgecho01.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

