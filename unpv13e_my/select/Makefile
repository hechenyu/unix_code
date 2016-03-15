# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	tcpcli01 tcpcli02 tcpcli03

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcpcli01: tcpcli01.o strcliselect01.o ../lib/readline.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli02: tcpcli02.o strcliselect02.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcpcli03: tcpcli03.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

