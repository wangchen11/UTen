#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

#define MTU  1500
#define BUFFER_MAX (MTU*2)
#define DEFAULT_SELECT_TIME_OUT_MS (10*1000)

class BaseUdpRadio {
private:
    int    port;
    int    socketFd;
    fd_set selectFdSet;
    struct timeval selectTimeOut;
    char   readBuffer[BUFFER_MAX];
    char   sendBuffer[BUFFER_MAX];

    bool bind();

    bool unbind();

public:
    BaseUdpRadio(int port = 0, int selectTimeOutMs = DEFAULT_SELECT_TIME_OUT_MS);

    ~BaseUdpRadio();

    inline void setSelectTimeOut(int ms) { 
        selectTimeOut.tv_sec  = ms/1000;
        selectTimeOut.tv_usec = ms%1000 * 1000;
    };

    inline bool isBinded() { return socketFd > 0; };

    bool step();

    bool handlePkg(sockaddr_in& from, char* buffer, int len);
};