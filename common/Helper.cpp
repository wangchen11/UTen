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
        if (in.sa_family == AF_INET) {
            struct sockaddr_in* recvAddrV4 = (struct sockaddr_in*)&in;
            snprintf(out.host, sizeof(out.host), "%s", inet_ntoa(*(struct in_addr*)&recvAddrV4->sin_addr.s_addr));
            out.port = ntohs(recvAddrV4->sin_port);
            return true;
        } else if (in.sa_family == AF_INET6) {
            TLOGE("IPV6 not implement.");
            out.host[0] = 0;
            out.port = 0;
        }
        return false;
    }
};