#ifndef _THREAD_H_
#define _THREAD_H_
#include <pthread.h>

class Thread
{
public:

    Thread();

    virtual ~Thread();

    void start(void *arg);

    void stop();

    virtual void run(void *arg) = 0;

protected:

    pthread_t threadID;

private:

    static void *threadRun(void *);

    void *arg;
};

#endif //_THREAD_H_
