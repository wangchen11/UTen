
CFLAGS += -Wall

common_objs := \
	common/BaseUdpRadio.o 

test_udp_radio: test/TestUdpRadio.o ${common_objs}

clean:
	rm -rf ${common_objs} test/TestUdpRadio.o