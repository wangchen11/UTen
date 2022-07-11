#pragma once

///////////////////////////////////////////////////////////
// server:
// insider:
// outsidder:
///////////////////////////////////////////////////////////

#include <stdint.h>
#include "CompileTime.h"

#define U_TEN_MAGIC_CODE       (uint32_t)(('U'<<0) | ('T'<<8) | ('e'<<16) | ('n')<<24)
#define U_TEN_PROTOCOL_MIN_LEN sizeof(struct UTenProtocol)

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
    UTEN_TYPE_MEET_OUTSIDER_RESPONSE,
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
    int32_t           magicCode; // must be U_TEN_MAGIC_CODE
    int16_t           checkSum;  // full package check sum
    enum UTenTypeCode type:16;   // one of enum UTenTypeCode
    int16_t           dataSize;  // size of data
    char              data[0];
};

///////////////////////////////////////////////////////////
// sub struct
///////////////////////////////////////////////////////////

// Insider To Server
struct UTenReportInsiderRequest {
    uint32_t serisId;
    uint64_t identifierCode;
};

// Server To Insider
struct UTenReportInsiderResponse {
    uint32_t              serisId;
    uint64_t              identifierCode;
};

// Outsider To Server
struct UTenMeetInsiderRequest {
    uint32_t serisId;
    uint64_t identifierCode;
};

// #define MAX_ADDRESS_LEN 128
struct UTenNetAddr {
    in_port_t	   sin_port;	/* Port number			*/
    struct in_addr sin_addr;	/* Internet address		*/
};

// Server To Outsider
struct UTenMeetInsiderResponse {
    uint32_t              serisId;
    uint64_t              identifierCode;   // zero if to Insider. Insider's code if to Outsider
    enum UTenResponseCode respCode:16;
    UTenNetAddr           address;
};

// Server To Insider
struct UTenMeetOutsiderResponse {
    UTenNetAddr           address;
};
#pragma pack ()

// TODO assert is little endian or translate to little endian

#define UTEN_DATA_SIZE()