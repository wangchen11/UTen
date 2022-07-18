#include <assert.h>
#include "TinyLog.h"
#include "BaseUdpRadio.h"

BaseUdpRadio::BaseUdpRadio(int port, int selectTimeOutMs): 
    port(port),
    socketFd(-1),
    selectTimeOutMs(selectTimeOutMs)
{
    FD_ZERO(&selectFdSet);
    bind();
}

BaseUdpRadio::~BaseUdpRadio() {
    unbind();
    FD_ZERO(&selectFdSet);
}

bool BaseUdpRadio::bind() {
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketFd==0) {
        perror("create socket failed");
        return false;
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port=htons(port);
    
    if(::bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) == 0) {
        FD_SET(socketFd, &selectFdSet);
        TLOGD("bind to port %d success.", ntohs(serverAddr.sin_port));
        return true;
    } else {
        TLOGE("bind to port *%d failed.", port);
        perror("bind failed");
        close(socketFd);
        socketFd = -1;
        return false;
    }
}

bool BaseUdpRadio::unbind() {
    if (socketFd > 0) {
        FD_CLR(socketFd, &selectFdSet);
        close(socketFd);
        socketFd = -1;
    }
    return true;
}

bool BaseUdpRadio::step() {
    struct sockaddr recvAddr;
    struct timeval  selectTimeOut;
    socklen_t recvAddrLen = sizeof(recvAddr);

    if (!isBinded()) {
        return false;
    }
    selectTimeOut.tv_sec  = selectTimeOutMs / 1000;
    selectTimeOut.tv_usec = (selectTimeOutMs%1000) / 1000;
    
    int selectRet = select(socketFd + 1, &selectFdSet, NULL, NULL, &selectTimeOut);
    if (selectRet > 0) {
        assert(selectRet == 1);
    } else if (selectRet == 0) {
        bool ret = handleSelectTimeOut();
        if (!ret) {
            TLOGD("handleSelectTimeOut");
        }
        return ret;
    } else { // selectRet<0
        TLOGE("select error");
        perror("select error");
        return false;
    }

    // int recvfrom(int s, void *buf, int len, unsigned int flags, struct sockaddr *from, socklen_t *fromlen);
    int recvLen = recvfrom(socketFd, readBuffer, MTU, MSG_DONTWAIT, &recvAddr, &recvAddrLen);
    if (recvLen>0) {
        readBuffer[recvLen] = 0;
        if (recvAddr.sa_family == AF_INET) {
            struct sockaddr_in* recvAddrV4 = (struct sockaddr_in*)&recvAddr;
            TLOGD("[recv from %s:%d] %s",
                inet_ntoa(*(struct in_addr*)&recvAddrV4->sin_addr.s_addr),
                ntohs(recvAddrV4->sin_port),
                readBuffer);
        } else if (recvAddr.sa_family == AF_INET6) {

            TLOGD("[recv from IPV6] %s", readBuffer);
        }
        

        handlePkg(socketFd, recvAddr, readBuffer, recvLen);
        return true;
    } else if (recvLen==0) {
        TLOGD("[recvLen = 0]");
    } else {
        TLOGD("[recvLen = %d]", recvLen);
    }
    return false;
}

bool BaseUdpRadio::handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len) {
    return true;
}

bool BaseUdpRadio::handleSelectTimeOut() {
    return true;
}