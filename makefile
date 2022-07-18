CXX    ?= g++
CFLAGS += -Wall

common_objs := \
	common/Helper.o \
	common/TinyLog.o \
	common/RateLimit.o \
	common/EventHub.o \
	common/BaseUdpRadio.o \
	common/ProtocolProcessor.o \

server_private_objs := \
	server/UTenServer.o \
	server/UTenInsiderPool.o

server_all_objs := ${common_objs} ${server_private_objs}
all_objs        := ${common_objs} ${server_private_objs}

all: test_udp_radio test_server

test_udp_radio: ${common_objs} test/TestUdpRadio.o
	${CXX} -o $@ $^

test_server: ${server_all_objs} test/TestServer.o
	${CXX} -o $@ $^

clean:
	@rm -vf ${all_objs} test_udp_radio test_server *.exe test/*.o
	@rm -rvf tiny