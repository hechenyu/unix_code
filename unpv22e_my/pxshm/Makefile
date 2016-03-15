# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	shmcreate shmunlink shmread shmwrite test1 test2 test3 \
		client1 server1 client2 server2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

client1: client1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

server1: server1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

client2: client2.o ../lib/sleep_us.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

server2: server2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmcreate: shmcreate.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmunlink: shmunlink.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmread: shmread.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmwrite: shmwrite.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1: test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2: test2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test3: test3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)


