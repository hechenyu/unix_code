# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -DDEBUG
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	test1 test2 test3

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"


test1:	test1.o ../lib/signal_rt.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2:	test2.o ../lib/signal_rt.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test3:	test3.o ../lib/signal_rt.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

