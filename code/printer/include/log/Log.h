#ifndef _LOG_H_
#define _LOG_H_
#include <iostream>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

#define LOG_ERROR(format, ...) \
    Log::getInstance().writeLog(Log::LogError, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOG_WARNING(format, ...) \
    Log::getInstance().writeLog(Log::LogWarning, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOG_NOTICE(format, ...) \
    Log::getInstance().writeLog(Log::LogNotice, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...) \
    Log::getInstance().writeLog(Log::LogInfo, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOG_DEBUG(format, ...) \
    Log::getInstance().writeLog(Log::LogDebug, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

class Log
{
public:

    #define LOG_BUF_LEN 2048
    #define LOG_FILE_NAME_LEN 256
    
    enum LogLevel
    {
        LogError,
        LogWarning,
        LogNotice,
        LogInfo,
        LogDebug
    };

    static Log& getInstance();
    
    bool initLog(const char *fileName, LogLevel level);
    
    void writeLog(LogLevel level, const char *file, const char *function, int line,
                    const char *format, ...);
    
    void writeLogNoLock(LogLevel level, const char *file, const char *function, int line,
                            const char *format, ...);
    
    void writeLogBuf(LogLevel level, const char *file, const char *function, int line,
                        const char *format, va_list valst);
    
    void setLogLevel(LogLevel level);

private:
    Log();

    class Clean
    {
    public:
        ~Clean();
    };

    FILE *pLogFd;
    
    char *pLogBuf;
    
    char *fileName;
    
    LogLevel curLogLevel;
    
    pthread_mutex_t mutex;

    static Clean clean;
    
    static Log instance;
};

#endif

