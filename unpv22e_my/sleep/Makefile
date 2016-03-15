# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	test1

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

test1:	test1.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

