# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	sendrecv udpcli05 udpcli06 udpserv01

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

sendrecv:	main.o send.o recv.o ../lib/udp_client.o ../lib/mcast_join.o ../lib/mcast_set_loop.o ../lib/sock_ntop.o ../lib/family_to_level.o ../lib/sockfd_to_family.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Version in book.
udpcli01:	udpcli01.o dgclibcast1.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Correct version using sigsetjmp()/siglongjmp().
udpcli05:	udpcli05.o dgclimcast5.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Try to bind multicast address and send.
udpcli06:	udpcli06.o dgclimcast6.o ../lib/sock_ntop_host.o ../lib/udp_client.o ../lib/sock_set_port.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

udpserv01:	udpserv01.o ../lib/dg_echo.o ../lib/mcast_join.o ../lib/family_to_level.o 
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

