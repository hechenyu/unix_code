# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = -lpthread
LDPATH = 

PROGS =	mysdr

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

mysdr:	main.o loop.o ../lib/udp_client.o ../lib/mcast_join.o ../lib/sock_ntop.o ../lib/family_to_level.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

