# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	checkopts prdefaults rcvbuf rcvbufset sockopt

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

checkopts: checkopts.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prdefaults: prdefaults.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

rcvbuf: rcvbuf.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

rcvbufset: rcvbufset.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

sockopt: sockopt.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

