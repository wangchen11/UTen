#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include "UTenProtocol.h"

template <typename T>
class ProtocolPackage {
public:
    sockaddr_in&         from;
    struct UTenProtocol* pkg;
    T*                   data;
    ProtocolPackage(sockaddr_in& from, struct UTenProtocol *pkg): from(from), pkg(pkg), data((T*)pkg->data) {
    }

    inline int dataSize() {
        return sizeof(data);
    }

    inline bool isRightSize(int dataSize) {
        return sizeof(data);
    }
};

class ProtocolProcessor {
public:
    ProtocolProcessor();

    ~ProtocolProcessor();

    virtual bool dispatchPackage(sockaddr_in& from, uint8_t* buffer, int len);

    virtual bool onPingRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onPingResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) = 0;
    virtual bool onReportInsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg) = 0;
    virtual bool onMeetInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) = 0;
    virtual bool onMeetInsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg) = 0;
    virtual bool onMeetOutsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) = 0;
    virtual bool onMeetOutsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg) = 0;
};