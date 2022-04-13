#include <assert.h>
#include "BaseUdpRadio.h"

BaseUdpRadio::BaseUdpRadio(int port, int selectTimeOutMs): 
    port(port),
    socketFd(-1)
{
    setSelectTimeOut(selectTimeOutMs);
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
        printf("bind to port %d success.\n", ntohs(serverAddr.sin_port));
        return true;
    } else {
        fprintf(stderr, "bind to port *%d failed.\n", port);
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
}

bool BaseUdpRadio::step() {
    struct sockaddr_in recvAddr;
    socklen_t recvAddrLen = sizeof(recvAddr);

    if (!isBinded()) {
        return false;
    }
    
    int selectRet = select(socketFd + 1, &selectFdSet, NULL, NULL, &selectTimeOut);
    if (selectRet > 0) {
        assert(selectRet == 1);
    } else if (selectRet == 0) {
        printf("[select time out]");
        return true;
    } else { // selectRet<0
        perror("select error");
        return false;
    }

    // int recvfrom(int s, void *buf, int len, unsigned int flags, struct sockaddr *from, socklen_t *fromlen);
    int recvLen = recvfrom(socketFd, readBuffer, MTU, MSG_DONTWAIT, (struct sockaddr*)&recvAddr, &recvAddrLen);
    if (recvLen>0) {
        readBuffer[recvLen] = 0;
        printf("[recv from %s:%d]%s\n",
                inet_ntoa(*(struct in_addr*)&recvAddr.sin_addr.s_addr),
                ntohs(recvAddr.sin_port),
                readBuffer);

        handlePkg(recvAddr, readBuffer, len);
        return true;
    } else if (recvLen==0) {
        printf("[recvLen = 0]\n");
    } else {
        printf("[recvLen = %d]\n", recvLen);
    }
    return false;
}

bool BaseUdpRadio::handlePkg(sockaddr_in& from, char* buffer, int len) {
}