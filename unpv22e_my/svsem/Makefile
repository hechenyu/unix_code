# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	semcreate semrmid semgetvalues semsetvalues semops semopsid \
		initval limits

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

semcreate:	semcreate.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semrmid:	semrmid.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semgetvalues:	semgetvalues.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semsetvalues:	semsetvalues.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semops:	semops.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semopsid:	semopsid.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

initval:	initval.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

limits:	limits.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

