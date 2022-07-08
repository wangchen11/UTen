
#include <stdio.h>
#include "ProtocolProcessor.h"

bool ProtocolProcessor::handlePackage(sockaddr_in& from, uint8_t* buffer, int len) {
    if (U_TEN_PROTOCOL_LEN != len) {
        printf("len(%d) is not equals U_TEN_PROTOCOL_LEN(%d)\n", len, U_TEN_PROTOCOL_LEN);
        return false;
    }
    struct UTenProtocol *pkg = (struct UTenProtocol *)buffer;
    if (pkg->magicCode != U_TEN_MAGIC_CODE) {
        printf("error magic code.\n");
        return false;
    }
    if (pkg->dataSize + sizeof(struct UTenProtocol) > len) {
        printf("error data size :%d. all len is:%d\n", pkg->dataSize, len);
        return false;
    }
    // TODO check sum
    
    switch (pkg->type)
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
        ProtocolPackage<UTenReportInsiderRequest> pkg(from, );
        onPingRequest(pkg);
        return true;
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


bool ProtocolProcessor::onPingRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return true;
}

bool ProtocolProcessor::onPingResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return true;
}