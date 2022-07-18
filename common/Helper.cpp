#include "Helper.h"
#include "TinyLog.h"

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
};