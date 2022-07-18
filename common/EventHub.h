#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include "Config.h"
#include "RateLimit.h"

class EventHub {
protected:
    int     selectTimeOutMs;
    uint8_t readBuffer[BUFFER_MAX];
    IntervalRateLimit timesUpLimit;
    
public:
    volatile bool alive;
    
    EventHub(int selectTimeOutMs = SELECT_TIME_OUT_MS);

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

    virtual void handleTimesUp();
};