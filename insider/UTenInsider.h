#pragma once

#include "../common/BaseUdpRadio.h"
#include "../common/ProtocolProcessor.h"

class UTenInsiderConfig {
public:
    sockaddr serverAddr;
    uint64_t identifierCode;
    int localPort = 0;
    int selectTimeOutMs;
    int keepConnectionIntervalMs;
    int keepConnectionTimeOutMs;

    UTenInsiderConfig() {
        identifierCode = 0;
        localPort = 0;
        selectTimeOutMs = SELECT_TIME_OUT_MS;
        serverAddr.sa_family = AF_UNSPEC;
        keepConnectionIntervalMs = KEEP_CONNECTION_INTERVAL_MS;
        keepConnectionTimeOutMs  = KEEP_CONNECTION_TIME_OUT_MS;
    }
};

class UTenInsider: public BaseUdpRadio, public ProtocolProcessor {
private:
    UTenInsiderConfig config;
    volatile bool connected;
    IntervalRateLimit reportSelfRateLimt;
    uint64_t offlineAt;

    inline void refreshConnection() {
        offlineAt = Helper::bootMs() + config.keepConnectionTimeOutMs;
    }

    inline bool isFreshConnection() {
        return Helper::bootMs() < offlineAt;
    }
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
