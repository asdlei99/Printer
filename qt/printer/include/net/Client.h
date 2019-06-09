#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <string>

class Client
{
public:

    Client();

    virtual ~Client();

    bool setIpAndPort(const char *ip, int port);

    bool setUserAndPsk(const char *user, const char *psk);

    virtual bool clientConnect() = 0;

    virtual bool clientDisconnect() = 0;

protected:

    std::string ip;
    int port;

    std::string user;
    std::string psk;
};

#endif //_CLIENT_H_