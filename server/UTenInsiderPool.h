#include <stdint.h>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../common/RateLimit.h"

class UTenInsiderData {
public:
    uint64_t           identifierCode;
    struct sockaddr_in recvAddr;
    struct timespec    recycleAfter;
};

class UTenInsiderPool {
private:
    IntervalRateLimit  recycleRateLimit;
    int                recycleStepCount;
    int                recycleAfterSec;
    std::map<uint64_t, UTenInsiderData> insiderMap;
public:
    /**
     * @brief Construct a new UTenInsiderPool object
     * 
     * @param recycleStepMs     do recycle each recycleStepMs
     * @param recycleAfterSec   recycle insider put after recycleAfterSec
     */
    UTenInsiderPool(int recycleStepMs, int recycleAfterSec);

    virtual ~UTenInsiderPool();
    virtual bool             put(uint64_t identifierCode, struct sockaddr_in &recvAddr);
    virtual UTenInsiderData* get(uint64_t identifierCode);
    virtual void             stepRecycle();
};