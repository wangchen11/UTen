#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

#define MTU          1500
#define MAX_PKG_SIZE (MTU-8-20-8) // MTU - 36
#define BUFFER_MAX   (MAX_PKG_SIZE*2)
#define DEFAULT_SELECT_TIME_OUT_MS (10*1000)

class EventHub {
protected:
    int     selectTimeOutMs;
    uint8_t readBuffer[BUFFER_MAX];
    
public:
    volatile bool alive;
    
    EventHub(int selectTimeOutMs = DEFAULT_SELECT_TIME_OUT_MS);

    virtual ~EventHub();

    virtual void step();

    /**
     * @brief 
     * 
     * @param selectFdSet 
     * @return int maxFd
     */
    virtual int  fillFdSet(fd_set &selectFdSet) = 0;
    
    virtual void handleFdEvent(fd_set &selectFdSet, int socketFd);

    virtual void handlePkg(int socketFd, sockaddr& from, uint8_t* buffer, int len) = 0;

    // return false will finish step loop
    virtual void handleSelectTimeOut();
};