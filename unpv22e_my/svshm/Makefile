# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	shmget shmrmid shmread shmreadid shmwrite limits

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

shmget:	shmget.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmrmid:	shmrmid.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmread:	shmread.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmreadid:	shmreadid.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

shmwrite:	shmwrite.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

limits:	limits.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

