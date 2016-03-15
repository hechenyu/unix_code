# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	lsif01 lsif02 prifinfo prmac

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

lsif01:	lsif01.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lsif02:	lsif02.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prifinfo:	prifinfo.o ../lib/get_ifi_info.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prmac:	prmac.o ../lib/get_ifi_info.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1:	test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

