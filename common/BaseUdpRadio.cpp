#include <assert.h>
#include "TinyLog.h"
#include "BaseUdpRadio.h"

BaseUdpRadio::BaseUdpRadio(int port, int selectTimeOutMs): 
    port(port),
    socketFd(-1),
    EventHub(selectTimeOutMs)
{
    alive = bind();
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

int BaseUdpRadio::fillFdSet(fd_set &selectFdSet) {
    FD_SET(socketFd, &selectFdSet);
    return socketFd;
}

void BaseUdpRadio::handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len) {
}
