
#include <stdio.h>
#include "ProtocolProcessor.h"

ProtocolProcessor::ProtocolProcessor() {
}

ProtocolProcessor::~ProtocolProcessor() {
}

bool ProtocolProcessor::dispatchPackage(int socketFd, sockaddr& from, uint8_t* buffer, int len) {
    if (len < U_TEN_PROTOCOL_MIN_LEN) {
        printf("len(%d) is less than U_TEN_PROTOCOL_MIN_LEN(%ld)\n", len, U_TEN_PROTOCOL_MIN_LEN);
        return false;
    }
    struct UTenProtocol *protocol = (struct UTenProtocol *)buffer;
    if (protocol->magicCode != U_TEN_MAGIC_CODE) {
        printf("error magic code.\n");
        return false;
    }
    if (protocol->dataSize + sizeof(struct UTenProtocol) > len) {
        printf("error data size :%d. all len is:%d\n", protocol->dataSize, len);
        return false;
    }
    // TODO check sum
    
    switch (protocol->type)
    {
    case UTEN_TYPE_REPORT_INSIDER_REQUEST:
        return true;
    case UTEN_TYPE_REPORT_INSIDER_RESPONSE:
        return true;
    case UTEN_TYPE_MEET_INSIDER_REQUEST:
        return true;
    case UTEN_TYPE_MEET_INSIDER_RESPONSE:
        return true;
    case UTEN_TYPE_MEET_OUTSIDER_REQUEST:
        return true;
    case UTEN_TYPE_MEET_OUTSIDER_RESPONSE:
        return true;
    case UTEN_TYPE_PING_REQUEST: {
        ProtocolPackage<UTenReportInsiderRequest> pkg(socketFd, from, protocol);
        return onPingRequest(pkg);
    }
    case UTEN_TYPE_PING_RESPONSE:
        return true;
    case UTEN_TYPE_SEEK_REQUEST:
        return true;
    case UTEN_TYPE_SEEK_RESPONSE:
        return true;
    case UTEN_TYPE_DPK:
        return true;
    case UTEN_TYPE_STREAM:
        return true;
    
    default:
        return false;
    }
    
    return false;
}

bool ProtocolProcessor::onPingRequest(ProtocolPackage<uint8_t> &pkg) {
    return false;
}

bool ProtocolProcessor::onPingResponse(ProtocolPackage<uint8_t> &pkg) {
    return false;
}

bool ProtocolProcessor::onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return false;
}

bool ProtocolProcessor::onReportInsiderResponse(ProtocolPackage<UTenReportInsiderResponse> &pkg) {
    return false;
}

bool ProtocolProcessor::onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg) {
    return false;
}

bool ProtocolProcessor::onMeetInsiderResponse(ProtocolPackage<UTenMeetInsiderResponse> &pkg) {
    return false;
}

bool ProtocolProcessor::onMeetOutsiderResponse(ProtocolPackage<UTenMeetOutsiderResponse> &pkg) {
    return false;
}