#pragma once

#include "../common/BaseUdpRadio.h"
#include "../common/ProtocolProcessor.h"

class UTenServer: public BaseUdpRadio, public ProtocolProcessor {
private:
public:
    UTenServer(int port = 0, int selectTimeOutMs = DEFAULT_SELECT_TIME_OUT_MS);
    
    virtual ~UTenServer();

    virtual bool handlePkg(sockaddr& from, uint8_t* buffer, int len);
    // return false will finish step loop
    virtual bool handleSelectTimeOut();
    virtual bool onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg);
};
