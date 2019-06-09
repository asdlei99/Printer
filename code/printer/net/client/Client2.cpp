#include <net/Client2.h>
#include <log/Log.h>
#include <net/FileTransfer.h>
#include <lib/Parse.h>
#include <print/PrinterManage.h>
#include <print/Printer.h>

Client2::Client2()
{
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    addrLen = sizeof(serverAddr);
    buf = new char[1024];
    buf1 = new char[1024];
    buf2 = new char[1024];

    pthread_mutex_init(&mutex, NULL);
}

Client2::~Client2()
{
    delete buf;
    delete buf1;
    delete buf2;

    pthread_mutex_destroy(&mutex);
}

bool Client2::setIpAndPort(const char *ip, int port)
{
    this->ip = std::string(ip);
    this->port = port;

    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr  = inet_addr(this->ip.c_str());

    return true;
}

bool Client2::connectServer()
{
    int len;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(socketFd, (const struct sockaddr *)&serverAddr, addrLen))
    {
        LOG_ERROR("Can't connect to server.\n");
        close(socketFd);
        return false;
    }

    len = send(socketFd, "connect", strlen("connect"), 0);
    if(len <= 0)
    {
        LOG_ERROR("Send error.\n");
        close(socketFd);
        return false;
    }
    
    len = recv(socketFd, buf, 1024, 0);
    if(len <= 0)
    {
        LOG_ERROR("Recv error.\n");
        close(socketFd);
        return false;
    }

    if(!strncmp(buf, "ok", strlen("ok")))
    {
        LOG_DEBUG("Connect secced.\n");
        return true;
    }

    close(socketFd);

    return false;
}

bool Client2::disconnect()
{
    int len;

    stop();
    getFile.stop();

    len = send(socketFd, "disconnect", strlen("disconnect"), 0);
    if(len <= 0)
    {
        LOG_ERROR("Send error.\n");
        close(socketFd);
        return false;
    }

    len = recv(socketFd, buf, 1024, 0);
    if(len <= 0)
    {
        LOG_ERROR("Recv error.\n");
        close(socketFd);
        return false;
    }

    if(!strncmp(buf, "ok", strlen("ok")))
    {
        LOG_DEBUG("Disconnect success.\n");
        close(socketFd);
        return true;
    }

    return false;
}

void Client2::interact()
{
    start(NULL);
    getFile.start(this);
}

void Client2::GetFile::run(void *arg)
{
    Client2 *client = (Client2 *)arg;
    std::string fileName;
    char buf[100];
    char filePath[100];

    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&client->mutex);
        if(client->fileList.empty())
        {
            pthread_mutex_unlock(&client->mutex);
            continue;
        }

        fileName = client->fileList.front();
        client->fileList.pop_front();
        pthread_mutex_unlock(&client->mutex);

        sprintf(filePath, "%s%s", Parse::filePath, fileName.c_str());
        sprintf(buf, "ftp://%s/%s", client->ip.c_str(), fileName.c_str());
        ftpGetFile(buf, filePath);

//        Printer::PrintTask printTask;

//        printTask.fileName = std::string(filePath);
//        PrinterManage::getInstance()->addTask(printTask);
    }
}

void Client2::run(void *arg)
{
    int len;

    while(1)
    {
        memset(buf, 0, 1024);
        memset(buf1, 0, 1024);
        memset(buf2, 0, 1024);

        len = recv(socketFd, buf, 1024, 0);
        if(len <= 0)
        {
            LOG_ERROR("Recv error.\n");
            close(socketFd);
            return;
        }

        sscanf(buf, "%s %s", buf1, buf2);
        if(!strcmp(buf1, "print"))
        {
            LOG_DEBUG("Print file.\n");
            pthread_mutex_lock(&mutex);
            fileList.push_back(std::string(buf2));
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            
        }
    }
}
