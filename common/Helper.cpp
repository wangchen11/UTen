#include "Helper.h"
#include "TinyLog.h"
#include <stdio.h>
#include <arpa/inet.h>

namespace Helper {
    bool send(int socketFd, sockaddr& to, const void* buf, size_t len) {
        if (socketFd > 0) {
            int ret = sendto(socketFd, buf, len, 0, &to, sizeof(to));
            if (ret != len) {
                TLOGE("sendto ret(%d) != len(%d)", ret, len);
            }
            return ret == len; 
        } else {
            TLOGE("sendto socketFd(%d) is invaild", socketFd);
        }
        return false;
    }

    
    bool toUTenNetAddr(sockaddr& in, UTenNetAddr& out) {
        memset(&out, 0, sizeof(out));
        if (in.sa_family == AF_INET) {
            struct sockaddr_in* recvAddrV4 = (struct sockaddr_in*)&in;
            snprintf(out.host, sizeof(out.host), "%s", inet_ntoa(*(struct in_addr*)&recvAddrV4->sin_addr.s_addr));
            out.port = ntohs(recvAddrV4->sin_port);
            out.stopChar = 0;
            return true;
        } else if (in.sa_family == AF_INET6) {
            TLOGE("IPV6 not implement.");
            out.host[0] = 0;
            out.port = 0;
            out.stopChar = 0;
        }
        return false;
    }

    bool toSockaddr(UTenNetAddr& in, sockaddr& out) {
        memset(&out, 0, sizeof(out));
        in.stopChar = 0;
        struct hostent* hostentPtr = gethostbyname(in.host);
        if (hostentPtr) {
            out.sa_family = hostentPtr->h_addrtype;
            memcpy(&out.sa_data, hostentPtr->h_addr_list[0], sizeof(out.sa_data));
            return true;
        } else {
            TLOGW("host not found:%s", in.host);
        }
        return false;
    }
};