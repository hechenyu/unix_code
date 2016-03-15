# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	ftok1 ftok

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

ftok1: ftok1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

ftok: ftok.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

