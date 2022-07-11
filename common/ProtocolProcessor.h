#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include "UTenProtocol.h"

template <typename T>
class ProtocolPackage {
public:
    sockaddr&            from;
    struct UTenProtocol* pkg;
    T*                   data;
    ProtocolPackage(sockaddr& from, struct UTenProtocol *pkg): from(from), pkg(pkg), data((T*)pkg->data) {
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

    virtual bool dispatchPackage(sockaddr& from, uint8_t* buffer, int len);

    virtual bool onPingRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onPingResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onReportInsiderResponse(ProtocolPackage<UTenReportInsiderResponse> &pkg);
    virtual bool onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg);
    virtual bool onMeetInsiderResponse(ProtocolPackage<UTenMeetInsiderResponse> &pkg);
    virtual bool onMeetOutsiderResponse(ProtocolPackage<UTenMeetOutsiderResponse> &pkg);
};