#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdlib.h>

namespace Common
{
    enum PrinterStatus
    {
        PrinterStatusUndefine = -1,
        PrinterStatusErr,
        PrinterStatusFree,
        PrinterStatusStart,
        PrinterStatusPrinting,
        PrinterStatusEnd,
    };

    struct CallBackTask {
        CallBackTask() {
            this->func = NULL;
            this->arg = NULL;
            this->taskSign = rand();;
        }

        bool operator == (const CallBackTask &task) {
            return this->taskSign == task.taskSign;
        }

        void (*func)(void *arg, void *info);
        void *arg;
        int taskSign;
    };

}

#endif //_COMMON_H_