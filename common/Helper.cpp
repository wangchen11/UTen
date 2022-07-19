#include "Helper.h"
#include "TinyLog.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/stat.h>

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

    
    bool toSockaddr(const char* host, int port, sockaddr& out) {
        UTenNetAddr in;
        snprintf(in.host, sizeof(in.host), "%s", host);
        in.port = port;
        in.stopChar = 0;
        return toSockaddr(in, out);
    }

    bool toSockaddr(UTenNetAddr& in, sockaddr& out) {
        memset(&out, 0, sizeof(out));
        in.stopChar = 0;
        struct hostent* hostentPtr = gethostbyname(in.host);
        if (hostentPtr) {
            out.sa_family = hostentPtr->h_addrtype;
            if (hostentPtr->h_addrtype == AF_INET) {
                sockaddr_in* outP = (sockaddr_in*)&out;
                outP->sin_family = hostentPtr->h_addrtype;
                outP->sin_port   = htons(in.port);
                memcpy(&outP->sin_addr.s_addr, hostentPtr->h_addr, sizeof(outP->sin_addr.s_addr));
                return true;
            } else if (hostentPtr->h_addrtype == AF_INET6) {
                TLOGW("toSockaddr unsupport IPV6:%s", in.host);
            } else {
                TLOGW("toSockaddr unsupport addrtype:%s", in.host);
            }
        } else {
            TLOGW("host not found:%s", in.host);
        }
        return false;
    }
    
    uint64_t bootMs() {
        struct timespec tsp;
        clock_gettime(CLOCK_REALTIME, &tsp);
        return tsp.tv_sec * 1000 + tsp.tv_nsec/1000/1000;
    }
};