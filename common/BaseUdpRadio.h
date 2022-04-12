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

class BaseUdpRadio {
private:
    int  port;
    int  socketFd;
    char readBuffer[BUFFER_MAX];
    char sendBuffer[BUFFER_MAX];

    bool bind();

    bool unbind();

public:
    BaseUdpRadio(int port = 0);

    ~BaseUdpRadio();

    bool step();

    bool handlePkg(sockaddr_in& from, char* buffer, int len);
};