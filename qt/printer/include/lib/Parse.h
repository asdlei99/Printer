#ifndef _PARSE_H_
#define _PARSE_H_
#include <vector>
#include <string>
#include <map>

class Parse
{
public:

    /*
     * 函数名称： parseConfigFile
     * 功能描述： 解析Printer.conf配置文件
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    static bool parseConfigFile();

    static char logPath[100];

    static char firmwarePath[100];

    static char filePath[100];

    static std::vector<std::string> printer;

    static std::map<std::string, std::string> printerFirmMap;

    static std::map<std::string, std::string> printerPpdMap;

private:

    Parse();

};

#endif //_PARSE_H_