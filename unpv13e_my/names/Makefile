# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	daytimetcpcli1 daytimetcpcli daytimetcpsrv1 daytimetcpsrv2 \
	daytimetcpcli2 daytimetcpcli3 daytimetcpsrv3 daytimetcpsrv4 \
	daytimeudpcli1 daytimeudpcli2 daytimeudpsrv2 daytimeudpsrv3 \
	hostent hostent2 prmyaddrs prmyaddrs1 test1 test2 testga

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

daytimetcpcli1: daytimetcpcli1.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli2: daytimetcpcli2.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli3: daytimetcpcli3.o ../lib/sock_ntop.o ../lib/sock_set_addr.o ../lib/sock_set_port.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpcli: daytimetcpcli.o ../lib/tcp_connect.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv1: daytimetcpsrv1.o ../lib/tcp_listen.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv2: daytimetcpsrv2.o ../lib/tcp_listen.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv3: daytimetcpsrv3.o ../lib/tcp_listen.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimetcpsrv4: daytimetcpsrv4.o ../lib/tcp_listen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimeudpcli1: daytimeudpcli1.o ../lib/udp_client.o ../lib/sock_ntop_host.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimeudpcli2: daytimeudpcli2.o ../lib/udp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimeudpsrv2: daytimeudpsrv2.o ../lib/udp_server.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

daytimeudpsrv3: daytimeudpsrv3.o udp_server_reuseaddr.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

hostent: hostent.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

hostent2: hostent2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prmyaddrs: prmyaddrs.o ../lib/my_addrs.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prmyaddrs1: prmyaddrs1.o myaddrs1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1: test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2: test2.o ../lib/udp_client.o ../lib/sock_ntop_host.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

testga: testga.o ../lib/sock_ntop.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)
