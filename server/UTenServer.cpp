#include "UTenServer.h"
#define MAX_INSIDER_COUNT 10*1000*1000

UTenServer::UTenServer(int port, int selectTimeOutMs): BaseUdpRadio(port, selectTimeOutMs),  ProtocolProcessor() {
}

UTenServer::~UTenServer() {
}

bool UTenServer::handlePkg(sockaddr& from, uint8_t* buffer, int len) {
    return ProtocolProcessor::dispatchPackage(from, buffer, len);
}

bool UTenServer::handleSelectTimeOut() {
    return true;
}

bool UTenServer::onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {

    return true;
}

bool UTenServer::onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg) {
    return true;
}