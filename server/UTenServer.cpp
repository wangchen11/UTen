#include "../common/TinyLog.h"
#include "UTenServer.h"
#define MAX_INSIDER_COUNT 10*1000*1000

UTenServer::UTenServer(int port, int selectTimeOutMs): BaseUdpRadio(port, selectTimeOutMs),  ProtocolProcessor() {
}

UTenServer::~UTenServer() {
}

bool UTenServer::handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len) {
    return ProtocolProcessor::dispatchPackage(socketFd, from, buffer, len);
}

bool UTenServer::handleSelectTimeOut() {
    return true;
}

bool UTenServer::onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    UTenReportInsiderResponse resp;
    resp.identifierCode = pkg.data->identifierCode;
    sendReportInsiderResponse(pkg.socketFd, pkg.from, resp);
    TLOGD("onReportInsiderRequest:%lld", pkg.data->identifierCode);
    return true;
}

bool UTenServer::onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg) {
    UTenMeetInsiderResponse resp;
    resp.identifierCode = pkg.data->identifierCode;
    // TODO resp.address = 
    // TODO resp.respCode = 
    sendMeetInsiderResponse(pkg.socketFd, pkg.from, resp);
    return true;
}