#include <stdint.h>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../common/RateLimit.h"

class UTenInsiderData {
public:
    uint64_t           identifierCode;
    struct sockaddr    recvAddr;
    uint64_t           offlineAt;
};

class UTenInsiderPool {
private:
    IntervalRateLimit  recycleRateLimit;
    int                recycleStepCount;
    int                recycleAfterMs;
    std::map<uint64_t, UTenInsiderData> insiderMap;
public:
    /**
     * @brief Construct a new UTenInsiderPool object
     * 
     * @param recycleStepMs     do recycle each recycleStepMs
     * @param recycleAfterMs   recycle insider put after recycleAfterMs
     */
    UTenInsiderPool(int recycleStepMs, int recycleAfterMs);

    virtual ~UTenInsiderPool();
    virtual bool             put(uint64_t identifierCode, struct sockaddr &recvAddr);
    virtual UTenInsiderData* get(uint64_t identifierCode);
    virtual void             stepRecycle();
};