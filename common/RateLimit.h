#pragma once

#include <stdint.h>
#include <time.h>

class RateLimit {
public:
    virtual bool allow() = 0;
};

/**
 * @brief eatch intervalMs only once allow
 * 
 */
class IntervalRateLimit: public RateLimit {
private:
    int             intervalMs;
    struct timespec allowAfter;
public:
    IntervalRateLimit(int intervalMs);

    virtual bool allow();
};