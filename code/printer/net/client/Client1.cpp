#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <net/Client1.h>
#include <unistd.h>
#include <log/Log.h>

/* 方法 */
#define GET         "GET"
#define ACK         "ACK"

/* 字段 */
#define ACTION      "ACTION"
#define FILE_TYPE   "FILE-TYPE"
#define FILE_PAHT   "FILE-PATH"
#define DATA_LEN    "DATA-LEN"
#define NUM         "NUM"

/* ACTION字段值 */
#define ACTION_CONNECT         "CONNECT"
#define ACTION_DISCONNECT      "DISCONNECT"
#define ACTION_ASK             "ASK"
#define ACTION_PRINT           "PRINT"
#define ACTION_PRINT_FINISH    "P-FINISH"

/* FILE-TYPE字段值 */
#define FILE_TYPE_TEXT    "TEXT"
#define FILE_TYPE_IMG     "IMG"
#define FILE_TYPE_PDF     "TEXT/PDF"
#define FILE_TYPE_BMP     "IMG/BMP"
#define FILE_TYPE_PNG     "IMG/PNG"
#define FILE_TYPE_JPG     "IMG/JPG"

/* 响应码 */
#define RCODE_OK                100

#define RCODE_CONNECT_FAIL      201
#define RCODE_DISCONNECT_FAIL   211

#define RCODE_REQUEST_PRIN_FAIL 301

/* 状态 */
#define STATUS_OK   "OK"
#define STATUS_FAIL "FAIL"

Client1::Client1()
{
    this->sock = -1;
    this->sendBufSize = 1024;
    this->sendBuf = new char[this->sendBufSize];
    this->recvBufSize = 1024;
    this->recvBuf = new char[this->recvBufSize];
    this->interatStatus = false;

    pthread_mutex_init(&this->mutex, NULL);
}

Client1::~Client1()
{
    if(this->sendBuf)
        delete[] this->sendBuf;

    if(this->recvBuf)
        delete[] this->recvBuf;

    pthread_mutex_destroy(&this->mutex);
}

bool Client1::clientConnect()
{
    if(this->ip.empty())
        goto _fail;

    if(this->sock != -1)
    {
        close(this->sock);
        this->sock = -1;
    }


    this->sock = this->createSocket();
    if(this->sock == -1)
    {
        LOG_ERROR("Create socket failure.\n");
        goto _fail;
    }

    if(this->actionConnect() != true)
    {
        LOG_ERROR("Connect to server failure.\n");
        goto _fail;
    }

    this->interatStatus = true;
    this->start(NULL);
    this->fileTransferThread.startRun(this);

    return true;

_fail:
    if(this->sock != -1)
    {
        close(this->sock);
        this->sock = -1;
    }

    return false;
}

bool Client1::clientDisconnect()
{
    this->interatStatus = false;
    this->stop();
    this->fileTransferThread.stopRun();

    return true;
}

int Client1::createSocket()
{
    int sock;
    struct sockaddr_in serverAddr;
    socklen_t addrlen;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        LOG_ERROR("Create socket failure.\n");
        goto _fail;
    }

    serverAddr.sin_family = AF_INET; //IPV4
    serverAddr.sin_port = htons(this->port); //端口号
    inet_pton(AF_INET, this->ip.c_str(), &serverAddr.sin_addr.s_addr); // ip地址
    addrlen = sizeof(struct sockaddr_in);

    if(connect(sock, (const struct sockaddr *)&serverAddr, addrlen) < 0)
    {
        LOG_ERROR("Connect failure.\n");
        goto _fail;
    }

    return sock;

_fail:
    if(sock > 0)
        close(sock);

    return -1;
}

void Client1::run(void *arg)
{
    this->interact();
}

void Client1::interact()
{
    while (this->interatStatus)
    {
        sleep(3);
        if(this->interatStatus == false)
            break;

        this->actionASK();
    }
}

bool Client1::askParse(char *buf, int len)
{
    char *start, *end;
    char *p;
    char action[20];

    if(!buf || len <= 0)
    {
        return false;
    }

    start = buf;
    end = buf+len-1;

    p = this->getLineFromBuf(start, end);
    if(!p || !strncmp((const char*)start, "\r\n", 2))
    {
        return false;
    }

    if(strncmp((const char*)start, GET"\r\n", strlen(GET"\r\n")))
    {
        return false;
    }

    start = p;
    p = this->getLineFromBuf(start, end);
    if(!p || !strncmp((const char*)start, "\r\n", 2))
    {
        return false;
    }

    if(strncmp((const char*)start, ACTION, strlen(ACTION)))
    {
        return false;
    }

    sscanf((const char *)start, ACTION":%s\r\n", action);
    start = p;
    
    if(!strcmp(action, ACTION_PRINT))
        return actionPrint(start, end-start+1);
    else if(!strcmp(action, ACTION_DISCONNECT))
        return actionDisconnect();
    else
    {
        LOG_DEBUG("Err ACTION.\n");
        return false;
    }
}

bool Client1::actionConnect()
{
    int len;

    sprintf(this->sendBuf, GET"\r\n"ACTION":"ACTION_CONNECT"\r\n"NUM":10\r\n\r\n");
    
    len = strlen(this->sendBuf);
    len = write(this->sock, this->sendBuf, len);
    if(len <= 0)
    {
        LOG_WARNING("Send msg to server failure.\n");
        return false;
    }

    len = read(this->sock, this->recvBuf, this->sendBufSize);
    if(len <= 0)
    {
        LOG_WARNING("Read err.\n");
        return false;
    }

    if(ackCode(this->recvBuf) != 100)
        return false;

    return true;
}

bool Client1::actionDisconnect()
{
    LOG_DEBUG("Disconnect.\n");

    this->ack(true, 100);

    return true;
}

bool Client1::actionASK()
{
    LOG_DEBUG("Action ask.\n");
    int len;
    int nfds = this->sock+1;
    fd_set rFdS;
    struct timeval timeout;

    FD_ZERO(&rFdS);
    FD_SET(this->sock, &rFdS);

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    sprintf(this->sendBuf, GET"\r\n"ACTION":"ACTION_ASK"\r\n\r\n");
    len = strlen(this->sendBuf);

    len = write(this->sock, this->sendBuf, len);
    if(len <= 0)
    {
        LOG_WARNING("Send msg to server failure.\n");
        return false;
    }

    if(select(nfds, &rFdS, NULL, NULL, &timeout) <= 0)
    {
        LOG_DEBUG("Select err.\n");
        return false;
    }

    len = read(this->sock, this->recvBuf, this->recvBufSize);
    if(len <= 0)
    {
        LOG_WARNING("read from socket err.\n");
        return false;
    }

    return askParse(this->recvBuf, len);
}

bool Client1::actionPrint(char *buf, int len)
{
    if(!buf)
        return false;

    char fileType[20];
    char filePath[200];
    char *start, *end;
    char *p;

    start = buf;
    end = start+len-1;

    sscanf(start, FILE_TYPE":%s\r\n", fileType);
    if(strcmp(fileType, FILE_TYPE_PDF) && strcmp(fileType, FILE_TYPE_BMP) &&
        strcmp(fileType, FILE_TYPE_PNG) && strcmp(fileType, FILE_TYPE_JPG))
        goto _fail;

    p = this->getLineFromBuf(start, end);
    if(!p || !strncmp(start, "\r\n", 2))
        goto _fail;

    start = p;
    sscanf(start, FILE_PAHT":%s\r\n", filePath);

    pthread_mutex_lock(&this->mutex);
    this->fileQueue.push(std::string(filePath));
    pthread_mutex_unlock(&this->mutex);

    this->ack(true, RCODE_OK);

    return true;

_fail:
    this->ack(false, RCODE_REQUEST_PRIN_FAIL);

    return false;
}

bool Client1::actionPrintFinish()
{
    this->interatStatus = false;
    close(this->sock);
    this->sock = -1;

    return true;
}

int Client1::ackCode(char *buf)
{
    char status[10];
    int rcode;

    if(strncmp(buf, ACK, strlen(ACK)))
        return -1;

    sscanf(buf, ACK" %d %s\r\n", &rcode, status);

    return rcode;
}

char *Client1::getLineFromBuf(char *start, char *end)
{
    char *p;
    p = start;

    while(p != end)
    {
        if(p[0] == '\r' && p[1] == '\n')
            return (p+2);

        p++;
    }
    
    return NULL;
}

bool Client1::ack(bool ok, int code)
{
    sprintf(this->sendBuf, ACK" %d %s\r\n\r\n", code, ok == true ? STATUS_OK : STATUS_FAIL);

    if(write(this->sock, this->sendBuf, strlen(this->sendBuf)) <= 0)
        return false;
    
    return true;
}

FileTransferThread::FileTransferThread()
{
    this->threadRun = false;

    this->fileTransfer = new Ftp;

    url = new char[1024];
}

FileTransferThread::~FileTransferThread()
{
    delete this->fileTransfer;
    delete url;
}

void FileTransferThread::run(void *arg)
{
    Client1 *client = (Client1 *)arg;

    while(this->threadRun)
    {
        sleep(3);
        pthread_mutex_lock(&client->mutex);

        if(!client->fileQueue.empty())
        {
            sprintf(url, "ftp://%s/%s", client->ip.c_str(), client->fileQueue.front().c_str());
            LOG_DEBUG("url: %s\n", url);

            if(this->fileTransfer->download(url, client->fileQueue.front().c_str()))
            {
                LOG_DEBUG("Download success.\n");
                client->fileQueue.pop();
            }
        }

        pthread_mutex_unlock(&client->mutex);
    }
}

void FileTransferThread::startRun(void *arg)
{
    this->threadRun = true;
    this->start(arg);
}

void FileTransferThread::stopRun()
{
    this->threadRun = false;
}
