CFLAGS += -Wall

common_objs := \
	common/BaseUdpRadio.o 

test_udp_radio: test/TestUdpRadio.o ${common_objs}
	g++ test/TestUdpRadio.o ${common_objs} -o test_udp_radio

clean:
	rm -rf ${common_objs} test/TestUdpRadio.o test_udp_radio