#include "UTenServer.h"

UTenServer::UTenServer(int port, int selectTimeOutMs): BaseUdpRadio(port, selectTimeOutMs),  ProtocolProcessor() {
}

UTenServer::~UTenServer() {
}

bool UTenServer::handlePkg(sockaddr_in& from, uint8_t* buffer, int len) {
    return ProtocolProcessor::dispatchPackage(from, buffer, len);
}

bool UTenServer::onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return false;
}

bool UTenServer::onReportInsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return false;
}

bool UTenServer::onMeetInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return false;
}

bool UTenServer::onMeetInsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return false;
}

bool UTenServer::onMeetOutsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return false;
}

bool UTenServer::onMeetOutsiderResponse(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    return false;
}