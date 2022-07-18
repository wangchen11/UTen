#pragma once

///////////////////////////////////////////////////////////
// server:
// insider:
// outsidder:
///////////////////////////////////////////////////////////

#include <stdint.h>
#include <string.h>
#include "Config.h"
#include "CompileTime.h"

#define U_TEN_MAGIC_CODE       (uint32_t)(('U'<<0) | ('T'<<8) | ('e'<<16) | ('n')<<24)
#define U_TEN_PROTOCOL_MIN_LEN sizeof(struct UTenProtocol)
#define MAX_DATA_SIZE (MAX_PKG_SIZE-sizeof(struct UTenProtocol))

#pragma pack (1)
///////////////////////////////////////////////////////////
// root struct
///////////////////////////////////////////////////////////
enum UTenTypeCode {
    UTEN_TYPE_MIN = 0,
    UTEN_TYPE_REPORT_INSIDER_REQUEST,
    UTEN_TYPE_REPORT_INSIDER_RESPONSE,
    UTEN_TYPE_MEET_INSIDER_REQUEST,
    UTEN_TYPE_MEET_INSIDER_RESPONSE,
    UTEN_TYPE_MEET_OUTSIDER_REQUEST,
    // ..... reserved
    UTEN_TYPE_PING_REQUEST = 20,
    UTEN_TYPE_PING_RESPONSE,
    UTEN_TYPE_SEEK_REQUEST,
    UTEN_TYPE_SEEK_RESPONSE,
    UTEN_TYPE_DPK,     // UDP
    UTEN_TYPE_STREAM,  // TCP
    UTEN_TYPE_MAX,
};

enum UTenResponseCode {
    SUCCESS = 0,
    FAILED  = 1,
};

struct UTenProtocol {
    uint32_t          magicCode; // must be U_TEN_MAGIC_CODE
    uint8_t           checkSum;  // full package check sum
    enum UTenTypeCode type:8;    // one of enum UTenTypeCode
    uint16_t          dataSize;  // size of data
    uint8_t           data[0];
public:
    inline void reset() {
        memset(this, 0, sizeof(*this));
        this->magicCode = U_TEN_MAGIC_CODE;
        this->type      = UTEN_TYPE_MIN;
    }

    inline size_t pkgSize() {
        return sizeof(*this) + dataSize;
    }

    inline uint16_t calCheckSum() {
        uint8_t   sum = 0;
        size_t    allSize = pkgSize();
        uint8_t*  ptr = (uint8_t*)this;

        for(int i = 0; i < allSize; i++) {
            sum += ptr[i];
        }
        sum -= checkSum;
        return sum;
    }

    inline void fullPkg(enum UTenTypeCode type, void* data, size_t dataSize) {
        this->reset();
        this->type = type;
        if (dataSize > MAX_DATA_SIZE) {
            dataSize = MAX_DATA_SIZE;
        }
        if (data) {
            memcpy(this->data, data, dataSize);
            this->dataSize = dataSize;
        } else {
            this->dataSize = 0;
        }
        
        this->checkSum = calCheckSum();
    }
};

///////////////////////////////////////////////////////////
// sub struct
///////////////////////////////////////////////////////////

// Insider To Server
struct UTenReportInsiderRequest {
    uint64_t identifierCode;
};

// Server To Insider
struct UTenReportInsiderResponse {
    uint64_t identifierCode;
};

// Outsider To Server
struct UTenMeetInsiderRequest {
    uint64_t identifierCode;
};

#define MAX_HOST_LEN 512
struct UTenNetAddr {
    uint16_t  port;
    char      host[MAX_HOST_LEN];
};

// Server To Outsider
struct UTenMeetInsiderResponse {
    uint64_t              identifierCode;
    enum UTenResponseCode respCode:16;
    UTenNetAddr           address;
};

// Server To Insider
struct UTenMeetOutsiderRequest {
    uint64_t              identifierCode;
    UTenNetAddr           address;
};
#pragma pack ()

// TODO assert is little endian or translate to little endian

#define UTEN_DATA_SIZE()