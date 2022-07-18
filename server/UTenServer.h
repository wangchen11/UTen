#pragma once

#include "../common/BaseUdpRadio.h"
#include "../common/ProtocolProcessor.h"

class UTenServer: public BaseUdpRadio, public ProtocolProcessor {
private:
public:
    UTenServer(int port = 0, int selectTimeOutMs = DEFAULT_SELECT_TIME_OUT_MS);
    
    virtual ~UTenServer();

    virtual void handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len);
    virtual void handleSelectTimeOut();
    virtual bool onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg);
};
