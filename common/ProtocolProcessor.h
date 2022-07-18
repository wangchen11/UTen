#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include "BaseUdpRadio.h"
#include "UTenProtocol.h"
#include "Helper.h"

template <typename T>
class ProtocolPackage {
public:
    int                  socketFd;
    sockaddr&            from;
    struct UTenProtocol* pkg;
    T*                   data;
    ProtocolPackage(int socketFd, sockaddr& from, struct UTenProtocol *pkg): socketFd(socketFd), from(from), pkg(pkg), data((T*)pkg->data) {
    }

    inline size_t dataSize() {
        return pkg->dataSize;
    }
};

class ProtocolProcessor {
private:
    uint8_t sendBuffer[BUFFER_MAX];
public:
    ProtocolProcessor();

    ~ProtocolProcessor();

    virtual bool dispatchPackage(int socketFd, sockaddr& from, uint8_t* buffer, int len);
    virtual bool onPingRequest(ProtocolPackage<uint8_t>  &pkg);
    virtual bool onPingResponse(ProtocolPackage<uint8_t> &pkg);
    virtual bool onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg);
    virtual bool onReportInsiderResponse(ProtocolPackage<UTenReportInsiderResponse> &pkg);
    virtual bool onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg);
    virtual bool onMeetInsiderResponse(ProtocolPackage<UTenMeetInsiderResponse> &pkg);
    virtual bool onMeetOutsiderRequest(ProtocolPackage<UTenMeetOutsiderRequest> &pkg);
    
    virtual bool sendPingRequest(int socketFd, sockaddr& to, void* data = NULL, size_t len = 0);
    virtual bool sendPingResponse(int socketFd, sockaddr& to, void* data = NULL, size_t len = 0);
    virtual bool sendReportInsiderRequest(int socketFd, sockaddr& to, UTenReportInsiderRequest &pkg);
    virtual bool sendReportInsiderResponse(int socketFd, sockaddr& to, UTenReportInsiderResponse &pkg);
    virtual bool sendMeetInsiderRequest(int socketFd, sockaddr& to, UTenMeetInsiderRequest&pkg);
    virtual bool sendMeetInsiderResponse(int socketFd, sockaddr& to, UTenMeetInsiderResponse &pkg);
    virtual bool sendMeetOutsiderResquest(int socketFd, sockaddr& to, UTenMeetOutsiderRequest &pkg);
};