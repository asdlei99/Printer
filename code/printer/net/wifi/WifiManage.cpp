#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/WifiManage.h>
#include <log/Log.h>

WifiManage::WifiManage()
{
    this->devName = std::string("wlan0");

    buf = new char[1024];
    cmd = new char[1024];
}

WifiManage::~WifiManage()
{
    delete buf;
    delete cmd;
}

bool WifiManage::devInit()
{
    if(system("sudo nmcli r wifi on"))
        return false;

    return true;
}

bool WifiManage::devExit()
{
    if(system("sudo nmcli r wifi off"))
        return false;

    return true;
}

void WifiManage::setNetworkCard(std::string devName)
{
    this->devName = devName;
}

/*
 *  *  SSID                MODE   CHAN  RATE       SIGNAL  BARS  SECURITY  
 *  请求连接            Infra  6     54 Mbit/s  89      ▂▄▆█  WPA2   
 */

bool WifiManage::scanWifi(std::vector<WifiInfo> &wifiInfo)
{
    sprintf(cmd, "sudo nmcli d wifi rescan ifname %s", this->devName.c_str());
    if(system(cmd))
    {
        LOG_WARNING("%s err.\n", cmd);
        return false;
    }

    FILE *pFile = popen("sudo nmcli dev wifi", "r");
    bool first = true;
    char ssid[100];
    char mode[40];
    int chan;
    int rateNum;
    char rate[40];
    int signal;
    char bars[40];
    char security[40];
    WifiInfo info;

    wifiInfo.clear();

    while(fgets(buf, 1024, pFile) != NULL)
    {
        if(first)
        {
            first = false;
            continue;
        }

        if(buf[0] == '*')
            sscanf(buf, "* %s %s %d %d %s %d %s %s", ssid, mode, &chan, &rateNum, rate, &signal, bars, security);
        else
            sscanf(buf, "%s %s %d %d %s %d %s %s", ssid, mode, &chan, &rateNum, rate, &signal, bars, security);
        
        info.name = std::string(ssid);
        info.signal = signal;
        wifiInfo.push_back(info);
    }

    fclose(pFile);

    return true;
}

bool WifiManage::connect(const char *ssid, const char *psk)
{
    /* 查看wifi是否存在记录中 */
    FILE *pFile = popen("sudo nmcli connection show", "r");
    if(!pFile)
    {
        LOG_WARNING("nmcli connection show err.\n");
        return false;
    }

    bool find = false;

    while(fgets(buf, 1024, pFile) != NULL)
    {
        if(!strncmp(buf, ssid, strlen(ssid)))
        {
            find = true;
            break;
        }
    }

    pclose(pFile);

    /* 断开之前连接的wifi */
    disconnect();

    memset(buf, 0, 1024);
    if(find) // 如果存在，修改密码，启动网络
    {
        sprintf(cmd, "sudo nmcli connection modify %s wifi-sec.psk %s", ssid, psk);
        if(system(cmd))
            return false;

        sprintf(cmd, "sudo nmcli connection up %s ifname %s", ssid, devName.c_str());
        pFile = popen(cmd, "r");
        if(!pFile)
        {
            printf("%s err.\n", cmd);
            return false;
        }

        if(fread(buf, 1, 1024, pFile) <= 0)
            return false;

        buf[1023] = '\0';
        pclose(pFile);

        if(!strstr(buf, "successfully"))
            return false;
    }
    else //如果不存在，直接添加一个新网络
    {
        sprintf(cmd, "sudo nmcli device wifi connect %s password %s ifname %s", ssid, psk, devName.c_str());
        pFile = popen(cmd, "r");
        if(!pFile)
        {
            printf("%s err.\n", cmd);
            return false;
        }

        if(fread(buf, 1, 1024, pFile) <= 0)
            return false;

        buf[1023] = '\0';
        pclose(pFile);

        if(!strstr(buf, "successfully"))
            return false;
    }
    
    this->ssid = std::string(ssid);

    return true;
}

bool WifiManage::disconnect()
{
    std::string wifi;

    /* 判断是否已连接wifi */
    if(status(wifi) == false)
        return false;

    /* 如果已连接，就断开 */
    sprintf(cmd, "sudo nmcli dev disconnect %s", devName.c_str());
    
    FILE *pFile = popen(cmd, "r");
    if(!pFile)
    {
        LOG_WARNING("%s err.\n", cmd);
        return false;
    }
    
    if(fread(buf, 1, 1024, pFile) <= 0)
        return false;
    buf[1023] = '\0';

    if(strstr(buf, "successfully"))
        return true;
    else
        return false;
}

/*
 *   DEVICE           TYPE      STATE         CONNECTION
 *   wlx70f11c26086b  wifi      disconnected  --
 *   ens33            ethernet  unmanaged     --
 *   lo               loopback  unmanaged     --
 */

bool WifiManage::status(std::string &wifi)
{
    FILE *pFile = popen("sudo nmcli device status", "r");
    if(!pFile)
    {
        LOG_WARNING("nmcli device status err.\n");
        return false;
    }

    char device[40];
    char type[40];
    char state[40];
    char ssid[100];

    while(fgets(buf, 1024, pFile) != NULL)
    {
        buf[1024] = '\0';
        if(strstr(buf, devName.c_str()))
        {
            sscanf(buf, "%s %s %s %s", device, type, state, ssid);
            if(!strcmp(state, "connected"))
            {
                wifi = std::string(ssid);
                return true;
            }
            else
                break;
        }
    }

    return false;
}