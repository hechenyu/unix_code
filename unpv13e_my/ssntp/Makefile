# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = -lpthread
LDPATH = 

PROGS =	ssntp

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

ssntp:	main.o sntp_proc.o ../lib/udp_client.o ../lib/sock_set_wild.o ../lib/get_ifi_info.o ../lib/mcast_join.o ../lib/sock_ntop.o ../lib/sock_set_addr.o ../lib/family_to_level.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

