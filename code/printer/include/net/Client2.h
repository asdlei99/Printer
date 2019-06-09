#ifndef _CLIENT2_H_
#define _CLIENT2_H_
#include <string>
#include <queue>
#include <list>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <lib/Thread.h>

class Client2 : public Thread
{

class GetFile : public Thread
{
public:
    virtual void run(void *arg);
};

public:

    Client2();

    ~Client2();

    bool setIpAndPort(const char *ip, int port);

    bool connectServer();

    bool disconnect();

    void interact();
    
    virtual void run(void *arg);

private:

    char *buf;

    char *buf1;

    char *buf2;

    std::string ip;

    int port;

    int socketFd;

    sockaddr_in serverAddr;

    socklen_t addrLen;

    std::list<std::string> fileList;

    pthread_mutex_t mutex;

    GetFile getFile;
};

#endif //_CLIENT2_H_