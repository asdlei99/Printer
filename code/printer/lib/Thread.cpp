#include <lib/Thread.h>

Thread::Thread()
{

}

Thread::~Thread()
{

}

void Thread::start(void *arg)
{
    this->arg = arg;

    pthread_create(&threadID, NULL, Thread::threadRun, this);
    pthread_detach(threadID);
}

void Thread::stop()
{
    pthread_cancel(threadID);
}

void *Thread::threadRun(void *arg)
{
    Thread *pThread = (Thread *)arg;

    pThread->run(pThread->arg);

    return NULL;
}

