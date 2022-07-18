
#include <stdio.h>
#include "TinyLog.h"
#include "ProtocolProcessor.h"

ProtocolProcessor::ProtocolProcessor() {
}

ProtocolProcessor::~ProtocolProcessor() {
}

bool ProtocolProcessor::dispatchPackage(int socketFd, sockaddr& from, uint8_t* buffer, int len) {
    if (len < U_TEN_PROTOCOL_MIN_LEN) {
        TLOGE("len(%d) is less than U_TEN_PROTOCOL_MIN_LEN(%ld)", len, U_TEN_PROTOCOL_MIN_LEN);
        return false;
    }
    struct UTenProtocol *protocol = (struct UTenProtocol *)buffer;
    if (protocol->magicCode != U_TEN_MAGIC_CODE) {
        TLOGE("error magic code.");
        return false;
    }
    if (protocol->dataSize + sizeof(struct UTenProtocol) > len) {
        TLOGE("error data size :%d. all len is:%d", protocol->dataSize, len);
        return false;
    }
    if (protocol->magicCode != U_TEN_MAGIC_CODE) {
        TLOGE("error magic code.");
        return false;
    }
    if (protocol->calCheckSum() != protocol->checkSum) {
        TLOGE("checkSum error, should be 0x%x but was 0x%x", protocol->calCheckSum(), protocol->checkSum );
        return false;
    }
    // TODO check sum
    
    switch (protocol->type)
    {
    case UTEN_TYPE_REPORT_INSIDER_REQUEST: {
        ProtocolPackage<UTenReportInsiderRequest> pkg(socketFd, from, protocol);
        return onReportInsiderRequest(pkg);
    }
    case UTEN_TYPE_REPORT_INSIDER_RESPONSE: {
        ProtocolPackage<UTenReportInsiderResponse> pkg(socketFd, from, protocol);
        return onReportInsiderResponse(pkg);
    }
    case UTEN_TYPE_MEET_INSIDER_REQUEST: {
        ProtocolPackage<UTenMeetInsiderRequest> pkg(socketFd, from, protocol);
        return onMeetInsiderRequest(pkg);
    }
    case UTEN_TYPE_MEET_INSIDER_RESPONSE: {
        ProtocolPackage<UTenMeetInsiderResponse> pkg(socketFd, from, protocol);
        return onMeetInsiderResponse(pkg);
    }
    case UTEN_TYPE_MEET_OUTSIDER_REQUEST: {
        ProtocolPackage<UTenMeetOutsiderResponse> pkg(socketFd, from, protocol);
        return onMeetOutsiderResponse(pkg);
    }
    case UTEN_TYPE_MEET_OUTSIDER_RESPONSE: {
        ProtocolPackage<UTenMeetOutsiderResponse> pkg(socketFd, from, protocol);
        return onMeetOutsiderResponse(pkg);
    }
    case UTEN_TYPE_PING_REQUEST: {
        ProtocolPackage<uint8_t> pkg(socketFd, from, protocol);
        return onPingRequest(pkg);
    }
    case UTEN_TYPE_PING_RESPONSE: {
        ProtocolPackage<uint8_t> pkg(socketFd, from, protocol);
        return onPingResponse(pkg);
    }
    case UTEN_TYPE_SEEK_REQUEST:
        return true;
    case UTEN_TYPE_SEEK_RESPONSE:
        return true;
    case UTEN_TYPE_DPK:
        return true;
    case UTEN_TYPE_STREAM:
        return true;
    
    default:
        TLOGE("unknown uten type:0x%x", protocol->type);
        return false;
    }
    
    return false;
}

bool ProtocolProcessor::onPingRequest(ProtocolPackage<uint8_t> &pkg) {
    TLOGD("onPingRequest");
    return sendPingResponse(pkg.socketFd, pkg.from, pkg.data, pkg.dataSize());
}

bool ProtocolProcessor::onPingResponse(ProtocolPackage<uint8_t> &pkg) {
    TLOGD("onPingResponse");
    return false;
}

bool ProtocolProcessor::onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    TLOGD("onReportInsiderRequest");
    return false;
}

bool ProtocolProcessor::onReportInsiderResponse(ProtocolPackage<UTenReportInsiderResponse> &pkg) {
    TLOGD("onReportInsiderResponse");
    return false;
}

bool ProtocolProcessor::onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg) {
    TLOGD("onMeetInsiderRequest");
    return false;
}

bool ProtocolProcessor::onMeetInsiderResponse(ProtocolPackage<UTenMeetInsiderResponse> &pkg) {
    TLOGD("onMeetInsiderResponse");
    return false;
}

bool ProtocolProcessor::onMeetOutsiderResponse(ProtocolPackage<UTenMeetOutsiderResponse> &pkg) {
    TLOGD("onMeetOutsiderResponse");
    return false;
}

bool ProtocolProcessor::sendPingRequest(int socketFd, sockaddr& to, void* data, size_t len) {
    UTenProtocol* protocol = (UTenProtocol*)&sendBuffer;
    protocol->fullPkg(UTEN_TYPE_PING_REQUEST, data, len);
    return Helper::send(socketFd, to, protocol, protocol->pkgSize());
}

bool ProtocolProcessor::sendPingResponse(int socketFd, sockaddr& to, void* data, size_t len) {
    UTenProtocol* protocol = (UTenProtocol*)&sendBuffer;
    protocol->fullPkg(UTEN_TYPE_PING_RESPONSE, data, len);
    return Helper::send(socketFd, to, protocol, protocol->pkgSize());
}

bool ProtocolProcessor::sendReportInsiderRequest(int socketFd, sockaddr& to, UTenReportInsiderRequest &pkg) {
    UTenProtocol* protocol = (UTenProtocol*)&sendBuffer;
    protocol->fullPkg(UTEN_TYPE_PING_RESPONSE, &pkg, sizeof(pkg));
    return Helper::send(socketFd, to, protocol, protocol->pkgSize());
}

bool ProtocolProcessor::sendReportInsiderResponse(int socketFd, sockaddr& to, UTenReportInsiderResponse &pkg) {
    UTenProtocol* protocol = (UTenProtocol*)&sendBuffer;
    protocol->fullPkg(UTEN_TYPE_PING_RESPONSE, &pkg, sizeof(pkg));
    return Helper::send(socketFd, to, protocol, protocol->pkgSize());
}

bool ProtocolProcessor::sendMeetInsiderRequest(int socketFd, sockaddr& to, UTenMeetInsiderRequest&pkg) {
    UTenProtocol* protocol = (UTenProtocol*)&sendBuffer;
    protocol->fullPkg(UTEN_TYPE_PING_RESPONSE, &pkg, sizeof(pkg));
    return Helper::send(socketFd, to, protocol, protocol->pkgSize());
}

bool ProtocolProcessor::sendMeetInsiderResponse(int socketFd, sockaddr& to, UTenMeetInsiderResponse &pkg) {
    UTenProtocol* protocol = (UTenProtocol*)&sendBuffer;
    protocol->fullPkg(UTEN_TYPE_PING_RESPONSE, NULL, 0);
    return Helper::send(socketFd, to, protocol, protocol->pkgSize());
}

bool ProtocolProcessor::sendMeetOutsiderResponse(int socketFd, sockaddr& to) {
    UTenProtocol* protocol = (UTenProtocol*)&sendBuffer;
    protocol->fullPkg(UTEN_TYPE_PING_RESPONSE, NULL, 0);
    return Helper::send(socketFd, to, protocol, protocol->pkgSize());
}