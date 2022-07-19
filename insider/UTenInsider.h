#pragma once

#include "../common/BaseUdpRadio.h"
#include "../common/ProtocolProcessor.h"

class UTenInsider: public BaseUdpRadio, public ProtocolProcessor {
public:
    UTenInsider(int port = 0, int selectTimeOutMs = SELECT_TIME_OUT_MS);
    
    virtual ~UTenInsider();

    virtual void handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len);
    virtual void handleTimesUp();
};
