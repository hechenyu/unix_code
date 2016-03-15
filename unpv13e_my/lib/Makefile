# my makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -c -g
INCLUDE =
LDFLAGS =
LDPATH =

TARGETS = writen.o readn.o readline.o str_cli.o str_echo.o signal.o \
	dg_cli.o dg_echo.o sock_ntop.o tcp_connect.o sock_ntop_host.o \
	tcp_listen.o udp_client.o udp_server.o daemon_init.o connect_timeo.o \
	read_fd.o write_fd.o gf_time.o connect_nonb.o \
	sock_set_addr.o sock_set_port.o udp_connect.o my_addrs.o \
	readable_timeo.o get_ifi_info.o sock_get_port.o sock_set_wild.o \
	host_serv.o sockfd_to_family.o family_to_level.o \
	mcast_get_if.o mcast_get_ttl.o mcast_leave.o mcast_set_loop.o \
	mcast_get_loop.o mcast_join.o mcast_set_if.o mcast_set_ttl.o rtt.o \
	sock_bind_wild.o
LIBNAME = libmyunpv1.a

.PHONY: all
all: $(TARGETS)
#	g++ -shared -o libpacket.so $(TARGETS)
	ar r $(LIBNAME) $(TARGETS)

.PHONY: clean
clean:
	$(RM) $(TARGETS) $(LIBNAME)

$(TARGETS): %.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@

