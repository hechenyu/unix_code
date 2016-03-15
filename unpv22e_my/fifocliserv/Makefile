# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -DDEBUG
INCLUDE = 
LDFLAGS =
LDPATH = 

PROGS =	mainclient mainserver

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

mainclient: mainclient.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mainserver: mainserver.o ../lib/readline.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

