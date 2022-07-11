#pragma once

#include <stdarg.h>

enum TINY_LOG_LEVEL {
    TINY_LOG_LEVEL_FATAL = 1,
    TINY_LOG_LEVEL_ERROR = 2,
    TINY_LOG_LEVEL_WARN  = 3,
    TINY_LOG_LEVEL_INFO  = 4,
    TINY_LOG_LEVEL_DEBUG = 5,
};

#define TLOG(level, fmt, ...) __tiny_log(__FILE__, __LINE__, level, fmt, ##__VA_ARGS__)

#define TLOGF(fmt, ...) TLOG(TINY_LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define TLOGE(fmt, ...) TLOG(TINY_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define TLOGW(fmt, ...) TLOG(TINY_LOG_LEVEL_WARN,  fmt, ##__VA_ARGS__)
#define TLOGI(fmt, ...) TLOG(TINY_LOG_LEVEL_INFO,  fmt, ##__VA_ARGS__)
#define TLOGD(fmt, ...) TLOG(TINY_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

extern int  __tiny_log (const char* file, int line, enum TINY_LOG_LEVEL level, const char* fmt, ...);
extern int  __tiny_vlog(const char* file, int line, enum TINY_LOG_LEVEL level, const char* fmt, va_list args);
extern const char* tinyLogDir;