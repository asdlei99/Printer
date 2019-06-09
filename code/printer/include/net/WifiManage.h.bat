#ifndef _WIFIMANAGE_H_
#define _WIFIMANAGE_H_
#include <string>
#include <vector>
#include <algorithm>

class WifiManage
{
public:

    struct WifiInfo {
        bool operator == (const WifiInfo &wifiInfo) {
            return this->hash == wifiInfo.hash;
        }

        std::string wifiName;
        std::string maxAddr;
        int freq;
        int sigLevel;
        std::string flags;
        int hash;
    };

    WifiManage();

    ~WifiManage();

    /*
     * 函数名称： devInit
     * 功能描述： 设备初始化
     * 输入参数： devName - 设备名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool devInit(const char *devName);

    /*
     * 函数名称： devExit
     * 功能描述： 设备退出
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： 无
     */ 
    void devExit();

    /*
     * 函数名称： scanWifi
     * 功能描述： 扫描当前环境的wifi热点
     * 输入参数： 无
     * 输出参数： wifiInfoVector - 当前环境的wifi热点
     * 返 回 值： 无
     */ 
    int scanWifi(std::vector<WifiInfo> &wifiInfoVector);

    /*
     * 函数名称： connectWifi
     * 功能描述： 连接wifi热点
     * 输入参数： wifiName - wifi热点名字
     * 输入参数： psk - wifi热点密码
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool connectWifi(const char *wifiName, const char *psk);

    /*
     * 函数名称： disconnectWifi
     * 功能描述： 断开wifi热点
     * 输入参数： wifiName - wifi热点名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool disconnectWifi(const char *wifiName);

    /*
     * 函数名称： isConnect
     * 功能描述： 判断是够已连接wifi
     * 输入参数： 无
     * 输出参数： wifiName - 已连接的名字
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool isConnect(std::string &wifiName);

private:

    /*
     * 函数名称： parseLine
     * 功能描述： 解析一行wifi信息的数据
     * 输入参数： line - 数据
     * 输出参数： wifiInfo - wifi信息
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool parseLine(char *line, WifiInfo *wifiInfo);

    /*
     * 函数名称： hash
     * 功能描述： 生成一个哈希值
     * 输入参数： str - 设备名
     * 输出参数： 无
     * 返 回 值： 哈希值
     */ 
    int hash(std::string &str);

    std::string devName;

    char *buf;

    char *bufTmp;

    char *cmd;

    char *line;
};

#endif //_WIFIMANAGE_H_