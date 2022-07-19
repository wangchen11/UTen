#include "../common/TinyLog.h"
#include "../common/Config.h"
#include "UTenInsider.h"
UTenInsider::UTenInsider(int port, int selectTimeOutMs):
    BaseUdpRadio(port, selectTimeOutMs)
{

}

UTenInsider::~UTenInsider() {

}

void UTenInsider::handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len) {

}

void UTenInsider::handleTimesUp() {

}