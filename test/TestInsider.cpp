#include <iostream>
#include "../common/TinyLog.h"
#include "../common/BaseUdpRadio.h"
#include "../insider/UTenInsider.h"

using namespace std;


int main() {
    TLOGD("test test:%s %d", "aaa", 123);
    UTenInsiderConfig config;
    config.identifierCode = 666;
    config.serverAddr;
    Helper::toSockaddr("127.0.0.1", 9002, config.serverAddr);
    UTenInsider insider(config);

    if (config.serverAddr.sa_family == AF_INET) {
        struct sockaddr_in* recvAddrV4 = (struct sockaddr_in*)&config.serverAddr;
        TLOGD("server is %s:%d",
            inet_ntoa(*(struct in_addr*)&recvAddrV4->sin_addr.s_addr),
            ntohs(recvAddrV4->sin_port));
    } else if (config.serverAddr.sa_family == AF_INET6) {
        TLOGD("server is  IPV6");
    }
    
    if (insider.isBinded()) {
        std::cout << "binded" << endl;
    } else {
        std::cout << "not binded" << endl;
    }
    insider.waitForConnected(20*1000);
    insider.stepLoop();
    
    std::cout << "finish" << endl;
    return 0;
}