#include "RateLimit.h"

IntervalRateLimit::IntervalRateLimit(int intervalMs) :
    intervalMs(intervalMs)
{
    allowAfter.tv_sec  = 0;
    allowAfter.tv_nsec = 0;
}

bool IntervalRateLimit::allow() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    bool allowNow = false;

    if (now.tv_sec > allowAfter.tv_sec) {
        allowNow = true;
    } else if (now.tv_sec > allowAfter.tv_sec) {
        allowNow = (now.tv_nsec > allowAfter.tv_nsec);
    } else {
        allowNow = false;
    }

    if (allowNow) {
        int afterMs = intervalMs + now.tv_nsec/1000/1000;
        allowAfter.tv_sec  = now.tv_sec + afterMs/1000;
        allowAfter.tv_nsec = (afterMs%1000) * 1000 * 1000;
    }
    return allowNow;
}