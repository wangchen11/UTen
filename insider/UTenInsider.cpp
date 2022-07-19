#include "../common/TinyLog.h"
#include "../common/Config.h"
#include "UTenInsider.h"
UTenInsider::UTenInsider(UTenInsiderConfig &config):
    config(config),
    connected(false),
    BaseUdpRadio(config.localPort, config.selectTimeOutMs)
{
    reportSelf();
}

UTenInsider::~UTenInsider() {
}

void UTenInsider::handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len) {
    dispatchPackage(socketFd, from, buffer, len);
}

void UTenInsider::handleTimesUp() {
    reportSelf();
    // TODO conneted
}

bool UTenInsider::reportSelf() {
    if (!isBinded()) {
        return false;
    }
    UTenReportInsiderRequest request;
    request.identifierCode = config.identifierCode;
    return sendReportInsiderRequest(socketFd, config.serverAddr, request);
}

bool UTenInsider::isHostConnected() {
    return connected;
}

bool UTenInsider::waitForConnected(int timeoutMs) {
    uint64_t timesup = Helper::bootMs() + timeoutMs;
    while (!isHostConnected()) {
        step();
        reportSelf();
        if (Helper::bootMs() >= timesup) {
            return false;
        }
    }
    return isHostConnected();
}

void UTenInsider::onConnectChanged(bool connected) {
    TLOGI("onConnectChanged:%s", connected ? "true" : "false");
}

bool UTenInsider::onReportInsiderResponse(ProtocolPackage<UTenReportInsiderResponse> &pkg) {
    if (pkg.data->identifierCode == config.identifierCode) {
        if (!connected) {
            connected = true;
            onConnectChanged(connected);
        }
        // TODO retime recycle time
    }
    return true;
}

bool UTenInsider::onMeetOutsiderRequest(ProtocolPackage<UTenMeetOutsiderRequest> &pkg) {
    return true;
}