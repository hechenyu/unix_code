# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	udpcli01 udpcli02 udpcli03 udpcli04 udpcli05 udpcli06

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

# Version in book.
udpcli01:	udpcli01.o dgclibcast1.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Incorrect version with alarm(1) and sleep(1) to tickle race condition.
udpcli02:	udpcli02.o dgclibcast2.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Incorrect version that blocks signals, but still has a race condition.
udpcli03:	udpcli03.o dgclibcast3.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Correct version using pselect().
udpcli04:	udpcli04.o dgclibcast4.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Correct version using sigsetjmp()/siglongjmp().
udpcli05:	udpcli05.o dgclibcast5.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Correct version using a pipe from signal handler to select().
udpcli06:	udpcli06.o dgclibcast6.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

