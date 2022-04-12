#include "BaseUdpRadio.h"

BaseUdpRadio::BaseUdpRadio(int port): 
    port(port),
    socketFd(-1)
{
    bind();
}

BaseUdpRadio::~BaseUdpRadio() {
    unbind();
}

bool BaseUdpRadio::bind() {
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
}

bool BaseUdpRadio::unbind() {
}

bool BaseUdpRadio::step() {
}

bool BaseUdpRadio::handlePkg(sockaddr_in& from, char* buffer, int len) {
}