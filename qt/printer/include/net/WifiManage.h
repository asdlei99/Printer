#ifndef _WIFI_MANAGE_H_
#define _WIFI_MANAGE_H_
#include <string>
#include <vector>

class WifiManage
{
public:
    
    struct WifiInfo {
        std::string name;
        int signal;
    };

    WifiManage();
    
    ~WifiManage();

    bool devInit();

    bool devExit();

    void setNetworkCard(std::string devName);

    bool scanWifi(std::vector<WifiInfo> &wifiInfo);

    bool connect(const char *ssid, const char *psk);

    bool disconnect();

    bool status(std::string &wifi);

private:

    std::string devName;

    std::string ssid;

    char *buf;

    char *cmd;
};



#endif //_WIFI_MANAGE_H_