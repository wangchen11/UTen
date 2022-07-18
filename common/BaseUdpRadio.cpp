#include <assert.h>
#include "TinyLog.h"
#include "BaseUdpRadio.h"

BaseUdpRadio::BaseUdpRadio(int port, int selectTimeOutMs): 
    port(port),
    socketFd(-1),
    selectTimeOutMs(selectTimeOutMs)
{
    bind();
}

BaseUdpRadio::~BaseUdpRadio() {
    unbind();
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
        close(socketFd);
        socketFd = -1;
    }
    return true;
}

bool BaseUdpRadio::step() {
    struct timeval  selectTimeOut;

    if (!isBinded()) {
        return false;
    }
    selectTimeOut.tv_sec  = selectTimeOutMs / 1000;
    selectTimeOut.tv_usec = (selectTimeOutMs%1000) / 1000;
    
    fd_set selectFdSet;
    FD_ZERO(&selectFdSet);
    int maxFd     = fillFdSet(selectFdSet);
    int selectRet = select(maxFd + 1, &selectFdSet, NULL, NULL, &selectTimeOut);
    if (selectRet > 0) {
        TLOGE("selectRet:%d", selectRet);
        for (size_t fd = 0; fd <= maxFd; fd++) {
            if (FD_ISSET(fd, &selectFdSet)) {
                handleFdEvent(selectFdSet, fd);
            }
        }
        return true;
    } else if (selectRet == 0) {
        bool ret = handleSelectTimeOut();
        TLOGD("handleSelectTimeOut:%d", ret);
        return ret;
    } else { // selectRet<0
        TLOGE("select error");
        perror("select error");
        return false;
    }
    return false;
}

int BaseUdpRadio::fillFdSet(fd_set &selectFdSet) {
    FD_SET(socketFd, &selectFdSet);
    return socketFd;
}
    
bool BaseUdpRadio::handleFdEvent(fd_set &selectFdSet, int socketFd) {
    if (socketFd != this->socketFd) {
        return false;
    }
    
    struct sockaddr recvAddr;
    socklen_t recvAddrLen = sizeof(recvAddr);
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