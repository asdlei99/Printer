#include <log/Log.h>
#include <sys/time.h>

//Log *Log::instance = NULL;

Log Log::instance;
Log::Clean Log::clean;

Log::Log()
{
    pLogBuf = new char[LOG_BUF_LEN];
    memset(pLogBuf, 0, LOG_BUF_LEN);
    
    fileName = new char[LOG_FILE_NAME_LEN];
    memset(fileName, 0, LOG_FILE_NAME_LEN);

    pLogFd = stdout;
    curLogLevel = LogDebug;
    pthread_mutex_init(&mutex, NULL);
}

#if 0
Log* Log::getInstance()
{
    if(instance == NULL)
    {
        instance = new Log;
    }

    return instance;
}
#endif

Log& Log::getInstance()
{
    return instance;
}

bool Log::initLog(const char * fileName, Log::LogLevel level)
{
    if(!fileName)
    {
        pLogFd = stdout;
    }
    else
    {
        strcpy(this->fileName, fileName);
        
        pLogFd = fopen(fileName, "a");
        if(!pLogBuf)
        {
            fprintf(stderr, "log init error!\n");
            return false;
        }
    }

    curLogLevel = level;

    return true;
}

void Log::writeLog(Log::LogLevel level, const char *file, const char *function, int line,
                    const char * format, ...)
{
    if(level > curLogLevel)
        return ;

    pthread_mutex_lock(&mutex);

    va_list valst;
    va_start(valst, format);

    writeLogBuf(level, file, function, line, format, valst);

    va_end(valst);

    fputs(pLogBuf, pLogFd);

    fflush(pLogFd);
    
    pthread_mutex_unlock(&mutex);
}

void Log::writeLogNoLock(Log::LogLevel level, const char *file, const char *function, int line,
                            const char * format, ...)
{
    if(level > curLogLevel)
        return ;

    va_list valst;
    va_start(valst, format);

    writeLogBuf(level, file, function, line, format, valst);

    va_end(valst);

    fputs(pLogBuf, pLogFd);

    fflush(pLogFd);

}


void Log::writeLogBuf(Log::LogLevel level, const char *file, const char *function, int line,
                        const char *format, va_list valst)
{
    char levelStr[10];

    switch (level)
    {
        case LogError:
            strcpy(levelStr, "Error");
        break;

        case LogWarning:
            strcpy(levelStr, "Warning");
        break;

        case LogNotice:
            strcpy(levelStr, "Notice");
        break;

        case LogInfo:
            strcpy(levelStr, "Info");
        break;

        case LogDebug:
            strcpy(levelStr, "Debug");
        break;
    }


    struct timeval now = {0, 0};
    gettimeofday(&now, NULL);
    struct tm* sys_tm = localtime(&(now.tv_sec));

    memset(pLogBuf, 0, LOG_BUF_LEN);

    int len = sprintf(pLogBuf, "%d-%02d-%02d %02d:%02d:%02d <%s> [%s:%s:%d]: ",
        sys_tm->tm_year + 1900, sys_tm->tm_mon + 1, sys_tm->tm_mday,
        sys_tm->tm_hour, sys_tm->tm_min, sys_tm->tm_sec,
        levelStr, file, function, line);

    vsnprintf(pLogBuf + len, LOG_BUF_LEN - len, format, valst);
}

void Log::setLogLevel(Log::LogLevel level)
{
    curLogLevel = level;
}

Log::Clean::~Clean()
{    
    if(Log::getInstance().pLogFd)
    {
        fclose(Log::getInstance().pLogFd);
            Log::getInstance().pLogFd = NULL;
            
        pthread_mutex_destroy(&Log::getInstance().mutex);
        delete Log::getInstance().pLogBuf;
    }
}

