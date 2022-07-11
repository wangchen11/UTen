#include <stdio.h>
#include <string.h>
#include <mutex>
#include <time.h>
#include <sys/stat.h>

#include "TinyLog.h"

#define MAX_LOG_LINE_SIZE      2048
#define MAX_LOG_FILE_PATH_SIZE 32
#define SPLIT_LOG_COUNT        100000

const char* tinyLogDir = "tiny/log";
static std::mutex logMutex;
static char       logBuffer[MAX_LOG_LINE_SIZE + 1];
static FILE* logOut = NULL;
static int countLog = 0;

static void __tiny_set_log_file_p(FILE* filep) {
    logMutex.lock();
    if (logOut && (logOut!=stdout) && (logOut!=stderr)) {
        fclose(logOut);
    }
    logOut = filep;
    logMutex.unlock();
}

static int __tiny_set_log_file_path(const char* path) {
    FILE* fp = fopen(path, "a");
    if (fp == NULL) {
        fprintf(stderr, "can not open log file:%s\n", path);
        return -1;
    }
    __tiny_set_log_file_p(fp);
    return 0;
}

static void mkdirs(const char* dir) {
    const char* splitIndex = strrchr(dir, '/');
    if (splitIndex) {
        int parentDirLen = splitIndex-dir;
        char *parent = (char*)malloc(parentDirLen+2);
        if (parent) {
            strncpy(parent, dir, parentDirLen);
            mkdirs(parent);
            free(parent);
        }
    }
    mkdir(dir, 0777);
}

static void new_log_file(struct tm *tmResult) {
    char logFile[100] = "";
    snprintf(logFile, sizeof(logFile), "%s/%d-%d-%d %02d-%02d-%02d",
        tinyLogDir,
        tmResult->tm_year,
        tmResult->tm_mon + 1,
        tmResult->tm_mday,
        tmResult->tm_hour,
        tmResult->tm_min,
        tmResult->tm_sec
    );
    mkdirs(tinyLogDir);
    __tiny_set_log_file_path(logFile);
}

int __tiny_log(const char* file, int line, enum TINY_LOG_LEVEL level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = __tiny_vlog(file, line, level, fmt, args);
    va_end(args);
    return ret;
}

int __tiny_vlog(const char* file, int line, enum TINY_LOG_LEVEL level, const char* fmt, va_list args) {
    struct timespec tsp;
    struct tm       tmResult;
    clock_gettime(CLOCK_REALTIME, &tsp);
    localtime_r(&tsp.tv_sec, &tmResult);
    logMutex.lock();
    if (countLog%SPLIT_LOG_COUNT==0) {
        new_log_file(&tmResult);
    }
    
    int fileNameLen   = strlen(file);
    int fileNameStart = 0;
    if (fileNameLen > MAX_LOG_FILE_PATH_SIZE) {
        fileNameStart = fileNameLen-MAX_LOG_FILE_PATH_SIZE;
    }
    
    logBuffer[0] = 0;
    int headSize    = snprintf(logBuffer, MAX_LOG_LINE_SIZE, "%d-%d %02d:%02d:%02d.%03d %s%s(%d): ",
        tmResult.tm_mon + 1,
        tmResult.tm_mday,
        tmResult.tm_hour,
        tmResult.tm_min,
        tmResult.tm_sec,
        (int)(tsp.tv_nsec/1000/1000),
        (fileNameStart==0) ? "" : "...",
        file + fileNameStart,
        line);
    int contentSize = vsnprintf(logBuffer + headSize, MAX_LOG_LINE_SIZE - headSize, fmt, args);
    printf("logOut is %p\n", logOut);
    if (logOut) {
        fputs(logBuffer, logOut);
        fputs("\n", logOut);
        //fflush(logOut);
    }
    countLog++;
    logMutex.unlock();
    return headSize + contentSize;
}
