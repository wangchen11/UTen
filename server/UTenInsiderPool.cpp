#include "UTenInsiderPool.h"
#include "../common/TinyLog.h"
#include "../common/Helper.h"

UTenInsiderPool::UTenInsiderPool(int recycleStepMs, int recycleAfterMs):
    recycleRateLimit(recycleStepMs),
    recycleAfterMs(recycleAfterMs),
    insiderMap()
{
}

UTenInsiderPool::~UTenInsiderPool() {
}

bool UTenInsiderPool::put(uint64_t identifierCode, struct sockaddr &recvAddr) {
    UTenInsiderData* data = get(identifierCode);
    if (data == NULL) {
        TLOGI("insert insider:%lld", identifierCode);
        data = &insiderMap[identifierCode];
    }
    data->identifierCode = identifierCode;
    data->recvAddr = recvAddr;
    data->offlineAt = Helper::bootMs() + recycleAfterMs;
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
            if (Helper::bootMs() >= data.offlineAt) {
                TLOGI("remove insider:%lld", data.identifierCode);
                insiderMap.erase(it++);
            } else {
                it++;
            }
        }
    }
}
