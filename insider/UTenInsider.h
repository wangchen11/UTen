#pragma once

#include "../common/BaseUdpRadio.h"
#include "../common/ProtocolProcessor.h"

class UTenInsiderConfig {
public:
    sockaddr serverAddr;
    uint64_t identifierCode;
    int localPort = 0;
    int selectTimeOutMs;

    UTenInsiderConfig() {
        identifierCode = 0;
        localPort = 0;
        selectTimeOutMs = SELECT_TIME_OUT_MS;
        serverAddr.sa_family = AF_UNSPEC;
    }
};

class UTenInsider: public BaseUdpRadio, public ProtocolProcessor {
private:
    UTenInsiderConfig config;
    volatile bool connected;
public:
    UTenInsider(UTenInsiderConfig &config);

    virtual ~UTenInsider();

    bool reportSelf();
    bool isHostConnected();
    bool waitForConnected(int timeoutMs);

    virtual void onConnectChanged(bool connected);

    virtual void handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len);
    virtual void handleTimesUp();
    
    virtual bool onReportInsiderResponse(ProtocolPackage<UTenReportInsiderResponse> &pkg);
    virtual bool onMeetOutsiderRequest(ProtocolPackage<UTenMeetOutsiderRequest> &pkg);
};
