#include <net/Client.h>

bool Client::setIpAndPort(const char *ip, int port)
{
    this->ip = std::string(ip);
    this->port = port;

    return true;
}

bool Client::setUserAndPsk(const char *user, const char *psk)
{
    this->user = std::string(user);
    this->psk = std::string(psk);

    return true;
}

Client::Client()
{

}

Client::~Client()
{


}

