#include "../common/TinyLog.h"
#include "../common/Config.h"
#include "UTenServer.h"
#define MAX_INSIDER_COUNT 10*1000*1000 

UTenServer::UTenServer(int port, int selectTimeOutMs): 
    BaseUdpRadio(port, selectTimeOutMs), 
    ProtocolProcessor(),
    insiderPool(RECYCLE_STEP_MS , RECYCLE_AFTER_MS)
{
}

UTenServer::~UTenServer() {
}

void UTenServer::handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len) {
    ProtocolProcessor::dispatchPackage(socketFd, from, buffer, len);
}

void UTenServer::handleTimesUp() {
    insiderPool.stepRecycle();
}

bool UTenServer::onReportInsiderRequest(ProtocolPackage<UTenReportInsiderRequest> &pkg) {
    // TLOGD("onReportInsiderRequest:%lld", pkg.data->identifierCode);
    bool ret = insiderPool.put(pkg.data->identifierCode, pkg.from);
    if (ret) {
        UTenReportInsiderResponse resp;
        resp.respCode = SUCCESS;
        resp.identifierCode = pkg.data->identifierCode;
        sendReportInsiderResponse(pkg.socketFd, pkg.from, resp);
    } else {
        TLOGE("can not put insider:%lld", pkg.data->identifierCode);
    }
    
    return true;
}

bool UTenServer::onMeetInsiderRequest(ProtocolPackage<UTenMeetInsiderRequest> &pkg) {
    UTenMeetInsiderResponse  resp;
    UTenMeetOutsiderRequest meetOutsiderResp;
    uint64_t identifierCode = pkg.data->identifierCode;
    UTenInsiderData* insider = insiderPool.get(resp.identifierCode);

    resp.identifierCode = identifierCode;
    meetOutsiderResp.identifierCode = identifierCode;
    if (insider != NULL) {
        resp.respCode         = SUCCESS;
        if(Helper::toUTenNetAddr(insider->recvAddr, resp.address)) {
            if(Helper::toUTenNetAddr(pkg.from, meetOutsiderResp.address)) {
                sendMeetOutsiderResquest(pkg.socketFd, insider->recvAddr, meetOutsiderResp);
            } else {
                TLOGE("onMeetInsiderRequest %lld pkg.from toUTenNetAddr failed", identifierCode);
                resp.respCode         = FAILED;
            }
        } else {
            TLOGE("onMeetInsiderRequest %lld insider->recvAddr toUTenNetAddr failed", identifierCode);
            resp.respCode         = FAILED;
        }
    } else {
        TLOGW("identifierCode %lld not online", identifierCode);
        resp.respCode         = FAILED;
    }
    sendMeetInsiderResponse(pkg.socketFd, pkg.from, resp);
    return true;
}