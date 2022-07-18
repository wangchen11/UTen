#include "UTenInsiderPool.h"
#include "../common/TinyLog.h"

UTenInsiderPool::UTenInsiderPool(int recycleStepMs, int recycleAfterSec):
    recycleRateLimit(recycleStepMs),
    recycleAfterSec(recycleAfterSec),
    insiderMap()
{
}

UTenInsiderPool::~UTenInsiderPool() {
}

bool UTenInsiderPool::put(uint64_t identifierCode, struct sockaddr &recvAddr) {
    UTenInsiderData &data = insiderMap[identifierCode];
    data.identifierCode = identifierCode;
    data.recvAddr = recvAddr;
    clock_gettime(CLOCK_MONOTONIC, &data.recycleAfter);
    data.recycleAfter.tv_sec += recycleAfterSec;
    return true;
}

UTenInsiderData* UTenInsiderPool::get(uint64_t identifierCode) {
    std::map<uint64_t, UTenInsiderData>::iterator it;
    it = insiderMap.find(identifierCode);
    if (it == insiderMap.end()) {
        return NULL;
    }
    return &(it->second);
}

void UTenInsiderPool::stepRecycle() {
    if (recycleRateLimit.allow()) {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        std::map<uint64_t, UTenInsiderData>::iterator it;
        it = insiderMap.begin();
        while (it != insiderMap.end()) {
            UTenInsiderData &data = it->second;
            bool recycleNow = false;

            if (now.tv_sec > data.recycleAfter.tv_sec) {
                recycleNow = true;
            } else if (now.tv_sec == data.recycleAfter.tv_sec) {
                recycleNow = (now.tv_nsec >= data.recycleAfter.tv_nsec);
            } else {
                recycleNow = false;
            }

            TLOGI("now is:%lld, remove insider at:%lld, recycleNow:%d", now.tv_sec, data.recycleAfter.tv_sec, recycleNow);
            if (recycleNow) {
                TLOGI("remove insider:%lld", data.identifierCode);
                insiderMap.erase(it++);
            } else {
                it++;
            }
        }
    }
}
