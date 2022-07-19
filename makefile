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

insider_private_objs := \
	insider/UTenInsider.o

server_all_objs  := ${common_objs} ${server_private_objs}
insider_all_objs := ${common_objs} ${insider_private_objs}
all_objs         := ${common_objs} ${server_private_objs} ${insider_private_objs}

all: test_udp_radio test_server test_insider

test_udp_radio: ${common_objs} test/TestUdpRadio.o
	${CXX} -o $@ $^

test_server: ${server_all_objs} test/TestServer.o
	${CXX} -o $@ $^

test_insider: ${insider_all_objs} test/TestInsider.o
	${CXX} -o $@ $^

clean:
	@rm -vf ${all_objs} test_udp_radio test_server test_insider *.exe test/*.o
	@rm -rvf tiny