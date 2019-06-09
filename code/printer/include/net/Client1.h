#ifndef _CLIENT1_H_
#define _CLIENT1_H_
#include <string>
#include <queue>
#include <pthread.h>
#include <stdint.h>
#include <net/Client.h>
#include <lib/Thread.h>
#include <net/FileTransfer.h>
#include <net/Ftp.h>

class FileTransferThread: public Thread
{
public:

    FileTransferThread();

    ~FileTransferThread();

    void startRun(void *arg);

    void stopRun();

    virtual void run(void *arg);

private:

    bool threadRun;

    FileTransfer *fileTransfer;

    char *url;
};

class Client1: public Client, public Thread
{
public:

    Client1();

    virtual ~Client1();

    virtual bool clientConnect();

    virtual bool clientDisconnect();

    virtual void run(void *arg);

private:

    friend class FileTransferThread;

    int createSocket();

    void interact();

    bool askParse(char *buf, int len);

    bool actionConnect();

    bool actionDisconnect();

    bool actionASK();

    bool actionPrint(char *buf, int len);

    bool actionPrintFinish();

    bool ack(bool ok, int code);

    int ackCode(char *buf);

    char *getLineFromBuf(char *start, char *end);

private:

    int sock;

    int sendBufSize;
    char *sendBuf;
    int recvBufSize;
    char *recvBuf;

    bool interatStatus;

    std::queue<std::string> fileQueue;
    pthread_mutex_t mutex;

    FileTransferThread fileTransferThread;
};

#endif //_CLIENT1_H_