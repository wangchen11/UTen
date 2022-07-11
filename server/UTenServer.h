#pragma once

#include "../common/BaseUdpRadio.h"
#include "../common/ProtocolProcessor.h"

class UTenServer: public BaseUdpRadio, public ProtocolProcessor {
private:
public:
    UTenServer(int port = 0, int selectTimeOutMs = DEFAULT_SELECT_TIME_OUT_MS);
    
    virtual ~UTenServer();

    virtual bool handlePkg(sockaddr_in& from, uint8_t* buffer, int len);
    
    virtual bool onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onReportInsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onMeetInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onMeetInsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onMeetOutsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onMeetOutsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg);
};
