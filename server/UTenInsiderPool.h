#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

class UTenInsiderData {
    uint64_t           identifierCode;
    struct sockaddr_in recvAddr;
};

class UTenInsiderPool {
public:
    /**
     * @brief Construct a new UTenInsiderPool object
     * 
     * @param recycleStepMs     do recycle each recycleStepMs
     * @param recycleStepCount  do recycle if it.recycleFlag > recycleStepCount
     */
    UTenInsiderPool(int recycleStepMs, int recycleStepCount);

    virtual ~UTenInsiderPool();
    virtual bool            put(uint64_t identifierCode, struct sockaddr_in &recvAddr);
    virtual UTenInsiderData get(uint64_t identifierCode);
    virtual bool            remove(uint64_t identifierCode);
};