#pragma once

#include "Config.h"
#include "EventHub.h"

class BaseUdpRadio: public EventHub {
private:
    int    port;
    int    socketFd;

    bool bind();

    bool unbind();

public:
    BaseUdpRadio(int port = 0, int selectTimeOutMs = SELECT_TIME_OUT_MS);

    virtual ~BaseUdpRadio();

    inline bool isBinded() { return socketFd > 0; };

    // @Override
    virtual int fillFdSet(fd_set &selectFdSet);

    // @Override
    virtual void handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len);
};
