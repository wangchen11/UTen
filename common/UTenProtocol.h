#pragma once

#include <stdint.h>
#include "CompileTime.h"

#define U_TEN_PROTOCOL_LEN 64

#pragma pack (1)
struct UTenProtocolRequest {
};

struct UTenProtocolResponse {
};

struct UTenProtocol {
    int16_t type;
    union {
        struct UTenProtocolRequest  request;
        struct UTenProtocolResponse response;
    };
    uint8_t reserved[60];
    int16_t checkSum;
};
#pragma pack ()

// TODO assert is little endian or translate to little endian
STATIC_ASSERT(sizeof(struct UTenProtocol)==U_TEN_PROTOCOL_LEN, "sizeof(UTenProtocol) must be U_TEN_PROTOCOL_LEN");


enum UTEN_TYPE_CODE {
    UTEN_TYPE_UNKNOWN = 0,
    UTEN_TYPE_REQUEST,
    UTEN_TYPE_RESPONSE,
    UTEN_TYPE_PING,
    UTEN_TYPE_SEEK,    // HOLE SEEK
    UTEN_TYPE_DPK,     // UDP
    UTEN_TYPE_STREAM,  // TCP
};
// common

// to server

// to client

// to insider

// to outsider