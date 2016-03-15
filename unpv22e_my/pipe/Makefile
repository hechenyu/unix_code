# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -DDEBUG
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	mainpipe mainpopen mainfifo \
		client_fifo server_fifo pipeconf test1 test2 fduplex fduplex2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

mainpipe: mainpipe.o client.o server.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mainpopen: mainpopen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mainfifo: mainfifo.o client.o server.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

client_fifo: client_main.o client.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

server_fifo: server_main.o server.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

pipeconf: pipeconf.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1: test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2: test2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

fduplex: fduplex.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

fduplex2: fduplex2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

