# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	byteorder daytimetcpcli daytimetcpcliv6 daytimetcpsrv1 daytimetcpsrv \
	daytimetcpcli1 daytimetcpcli2 daytimetcpcli3 daytimetcpsrv2 daytimetcpsrv3

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

byteorder: byteorder.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli: daytimetcpcli.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcliv6: daytimetcpcliv6.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv1: daytimetcpsrv1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv: daytimetcpsrv.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli1: daytimetcpcli1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli2: daytimetcpcli2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli3: daytimetcpcli3.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv2: daytimetcpsrv2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv3: daytimetcpsrv3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

