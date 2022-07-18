#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

#define MTU          1500
#define MAX_PKG_SIZE (MTU-8-20-8) // MTU - 36
#define BUFFER_MAX   (MAX_PKG_SIZE*2)
#define DEFAULT_SELECT_TIME_OUT_MS (10*1000)

class BaseUdpRadio {
private:
    int    port;
    int    socketFd;
    fd_set selectFdSet;
    int    selectTimeOutMs;
    uint8_t readBuffer[BUFFER_MAX];

    bool bind();

    bool unbind();

public:
    BaseUdpRadio(int port = 0, int selectTimeOutMs = DEFAULT_SELECT_TIME_OUT_MS);

    virtual ~BaseUdpRadio();

    inline bool isBinded() { return socketFd > 0; };

    virtual bool step();

    virtual bool handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len);

    // return false will finish step loop
    virtual bool handleSelectTimeOut();
};
