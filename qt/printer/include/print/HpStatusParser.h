#ifndef _HPSTATUSPARSER_H_
#define _HPSTATUSPARSER_H_
#include <print/PrinterStatusParser.h>
#include <common/Common.h>

class HpStatusParser : public PrinterStatusParser
{
public:

    enum MessageType
    {
        MessageTypeUndefine = -1,
        MessageTypeDevice,
        MessageTypeJob,
        MessageTypePage,
        MessageTypeTimed,
    };

    HpStatusParser();

    ~HpStatusParser();

    /*
     * 函数名称： parse
     * 功能描述： 解析打印机返回的参数
     * 输入参数： str - 一行参数
     * 输出参数： 无
     * 返 回 值： 打印机当前状态
     */
    virtual Common::PrinterStatus parse(char *str);

private:

    /*
     * 函数名称： 一下的行数
     * 功能描述： 解析对应类型的打印机返回参数
     * 输入参数： str - 一行参数
     * 输出参数： 无
     * 返 回 值： 打印机当前状态
     */
    Common::PrinterStatus parseDeviceInfo(char *str);

    Common::PrinterStatus parseJobInfo(char *str);

    Common::PrinterStatus parsePageInfo(char *str);

    Common::PrinterStatus parseTimedInfo(char *str);

    Common::PrinterStatus status;

    MessageType messageType;

};

#endif //_HPSTATUSPARSER_H_