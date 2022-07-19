#include "EventHub.h"
#include "TinyLog.h"

EventHub::EventHub(int selectTimeOutMs) :
    alive(true),
    selectTimeOutMs(selectTimeOutMs),
    timesUpLimit(selectTimeOutMs) {
}

EventHub::~EventHub() {
}

void EventHub::step() {
    struct timeval  selectTimeOut;

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
        if (timesUpLimit.allow()) {
            handleTimesUp();
        }
        
    } else if (selectRet == 0) {
        timesUpLimit.allow();
        handleTimesUp();
    } else { // selectRet<0
        TLOGE("select error");
        perror("select error");
    }
}

void EventHub::stepLoop() {
    while (alive) {
        step();
    }
}

void EventHub::handleFdEvent(fd_set &selectFdSet, int socketFd) {
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
    } else if (recvLen==0) {
        TLOGD("[recvLen = 0]");
    } else {
        TLOGD("[recvLen = %d]", recvLen);
    }
}

void EventHub::handleTimesUp() {
}