#include <net/WifiManage.h>
#include <stdio.h>
#include <unistd.h>
#include <log/Log.h>
#include <lib/BytesStream.h>

WifiManage::WifiManage()
{
    buf = new char[4096];

    bufTmp = new char[1024];

    cmd = new char[200];

    line = new char[1024];
}

WifiManage::~WifiManage()
{
    if(buf)
    {
        delete[] buf;
        buf = NULL;
    }

    if(bufTmp)
    {
        delete[] bufTmp;
        bufTmp = NULL;
    }

    if(line)
    {
        delete[] line;
        line = NULL;
    }

    if(cmd)
    {
        delete cmd;
        cmd = NULL;
    }
}

bool WifiManage::devInit(const char *devName)
{
    int ret;

    ret = wpaClientInit(devName);
    if(ret < 0)
    {
        LOG_ERROR("Wifi init error.\n");
        return false;
    }

    return true;
}

void WifiManage::devExit()
{
    wpaClientExit();
}

bool WifiManage::parseLine(char *line, WifiManage::WifiInfo *wifiInfo)
{
    if(!line)
        return false;

    char maxAddr[40];
    int freq;
    int sigLevel;
    char flags[100];
    char wifiName[100];

    strcpy(bufTmp, line);
    sscanf(bufTmp, "%s %d %d %s %s", maxAddr, &freq, &sigLevel, flags, wifiName);

    wifiInfo->maxAddr = std::string(maxAddr);
    wifiInfo->freq = freq;
    wifiInfo->sigLevel = sigLevel;
    wifiInfo->flags = std::string(flags);
    wifiInfo->wifiName = std::string(wifiName);
    wifiInfo->hash = hash(wifiInfo->wifiName);

    return true;
}

int WifiManage::hash(std::string &str)
{
    int ret = 1432;

    for(unsigned int i = 0; i < str.size(); i++)
        ret ^= ((int)str[i]<<((i%4)*8));

    return ret;
}

int WifiManage::scanWifi(std::vector<WifiInfo> &wifiInfoVector)
{
    int len;
    int num = 0;
    int first = 1;

    wifiInfoVector.clear();

    wpaRequest("scan", buf, &len);
    if(strncmp(buf, "OK", 2))
        return -1;

    wpaRequest("scan_result", buf, &len);

    char *pBuf = buf;
    WifiInfo wifiInfo;

    while(pBuf)
    {
        pBuf = BytesStream::getLineFromBuf(pBuf, line, &len);
        if(len <= 0)
            break;

        if(first)
        {
            first = 0;
            continue;
        }

        parseLine(line, &wifiInfo);

        if(find(wifiInfoVector.begin(), wifiInfoVector.end(), wifiInfo) != wifiInfoVector.end())
            continue;

        wifiInfoVector.push_back(wifiInfo);
        num++;
    }

    return num;
}

bool WifiManage::connectWifi(const char *wifiName, const char *psk)
{
    int ret;
    int len;
    int ap;
    int isExist = 0;
    int first = 1;
    char name[100];

    ret = wpaRequest("list_network", buf, &len);
    if(ret)
    {
        LOG_WARNING("List network error.\n");
        return false;
    }
    
    char *pBuf = buf;
    while(pBuf)
    {
        pBuf = BytesStream::getLineFromBuf(pBuf, line, &len);
        if(len <= 0)
            break;

        if(first)
        {
            first = 0;
            continue;
        }

        sscanf(line, "%d %s", &ap, name);

        if(!strcmp(wifiName, name))
        {
            sprintf(cmd, "disable_network %d", ap);
            ret = wpaRequest(cmd, buf, &len);
            if(ret)
            {
                LOG_WARNING("Disable network error.\n");
                return false;
            }

            isExist = 1;
            break;
        }
    }

    if(!isExist)
    {
        ret = wpaRequest("add_network", buf, &len);
        if(ret)
        {
            LOG_WARNING("Add network error.\n");
            return false;
        }

        sscanf(buf, "%d", &ap);

        sprintf(cmd, "set_network %d ssid \"%s\"", ap, wifiName);
        ret = wpaRequest(cmd, buf, &len);
        if(ret)
        {
            LOG_WARNING("Set network error.\n");
            return false;
        }
    }

    sprintf(cmd, "set_network %d psk \"%s\"", ap, psk);
    ret = wpaRequest(cmd, buf, &len);
    if(ret)
    {
        LOG_WARNING("Set network error.\n");
        return false;
    }

    sprintf(cmd, "select_network %d", ap);
    ret = wpaRequest(cmd, buf, &len);
    if(ret)
    {
        LOG_WARNING("Set network error.\n");
        return false;
    }

#if 0
    ret = wpaRequest("save_config", buf, &len);
    if(ret)
    {
        LOG_WARNING("Set network error.\n");
        return false;
    }
#endif

    return true;
}

bool WifiManage::disconnectWifi(const char *wifiName)
{
    int ret;
    int len;

    ret = wpaRequest("list_network", buf, &len);
    if(ret)
    {
        LOG_WARNING("List network error.\n");
        return false;
    }
    
    char *pBuf = buf;
    int first = 1;
    char name[100];
    int ap;
    while(pBuf)
    {
        pBuf = BytesStream::getLineFromBuf(pBuf, line, &len);
        if(len <= 0)
            break;

        if(first)
        {
            first = 0;
            continue;
        }

        sscanf(line, "%d %s", &ap, name);

        if(!strcmp(wifiName, name))
        {
            sprintf(cmd, "disable_network %d", ap);
            ret = wpaRequest(cmd, buf, &len);
            if(ret)
            {
                LOG_WARNING("Disable network error.\n");
                return false;
            }

            return true;
        }
    }

    return false;
}

bool WifiManage::isConnect(std::string &wifiName)
{
    int len;

    sprintf(cmd, "status");
    wpaRequest(cmd, buf, &len);

    if (strstr(buf, "COMPLETED"))
    {
        char *p = strstr(buf, "\nssid=");
        if(!p)
        {
            LOG_INFO("Can't find ssid.\n");
            return false;
        }

        p++;
        BytesStream::getLineFromBuf(p, cmd, &len);

        wifiName = std::string(cmd+strlen("ssid="));

        return true;
    }

    return false;
}
