#ifndef _PRINTERSTATUSPARSER_H_
#define _PRINTERSTATUSPARSER_H_
#include <common/Common.h>

class PrinterStatusParser
{
public:

    PrinterStatusParser();

    virtual ~PrinterStatusParser();

    /*
     * 函数名称： parse
     * 功能描述： 解析打印机返回的参数
     * 输入参数： str - 一行参数
     * 输出参数： 无
     * 返 回 值： 打印机当前状态
     */
    virtual Common::PrinterStatus parse(char *str) = 0;

};

#endif //_PRINTERSTATUSPARSER_H_