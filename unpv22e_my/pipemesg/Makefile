# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -DDEBUG
INCLUDE = 
LDFLAGS =
LDPATH = 

PROGS =	mainpipe mainfifo

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

mainpipe: mainpipe.o client.o server.o mesg_send.o mesg_recv.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mainfifo: mainfifo.o client.o server.o mesg_send.o mesg_recv.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

