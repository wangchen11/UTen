#pragma once
#include <sys/socket.h>
#include <netdb.h>
#include "UTenProtocol.h"

namespace Helper {
    bool send(int socketFd, sockaddr& to, const void* buf, size_t len);

    bool toUTenNetAddr(sockaddr& in, UTenNetAddr& out);
};