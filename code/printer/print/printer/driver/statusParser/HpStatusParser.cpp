#include <stdio.h>
#include <print/HpStatusParser.h>
#include <string.h>

HpStatusParser::HpStatusParser()
{
    status = Common::PrinterStatusUndefine;

    messageType = MessageTypeUndefine;

}
HpStatusParser::~HpStatusParser()
{

}

Common::PrinterStatus HpStatusParser::parseDeviceInfo(char *str)
{
    if(messageType != MessageTypeDevice)
        return Common::PrinterStatusUndefine;

    return Common::PrinterStatusUndefine;
}

Common::PrinterStatus HpStatusParser::parseJobInfo(char *str)
{
    if(messageType != MessageTypeJob)
        return Common::PrinterStatusUndefine;

    if(!strcmp(str, "START\r\n"))
        return Common::PrinterStatusStart;
    else if(!strcmp(str, "END\r\n"))
        return Common::PrinterStatusEnd;

    return Common::PrinterStatusUndefine;
}

Common::PrinterStatus HpStatusParser::parsePageInfo(char *str)
{
    if(messageType != MessageTypePage)
        return Common::PrinterStatusUndefine;

    return Common::PrinterStatusPrinting;
}

Common::PrinterStatus HpStatusParser::parseTimedInfo(char *str)
{
    if(messageType != MessageTypeTimed)
        return Common::PrinterStatusUndefine;

    return Common::PrinterStatusUndefine;
}

Common::PrinterStatus HpStatusParser::parse(char *str)
{
    if(!str)
        return Common::PrinterStatusUndefine;

    /* 先解析相应的类型 */
    if(str[0] == '\f' || messageType == MessageTypeUndefine)
    {
        if(str[0] == '\f')
            str++;

        if(strstr(str, "@PJL"))
        {
            if(strstr(str, "DEVICE"))
                messageType = MessageTypeDevice;
            else if(strstr(str, "JOB"))
                messageType = MessageTypeJob;
            else if(strstr(str, "PAGE"))
                messageType = MessageTypePage;
            else if(strstr(str, "TIMED"))
                messageType = MessageTypeTimed;
            else
                messageType = MessageTypeUndefine;
        }
        else
        {
            messageType = MessageTypeUndefine;
        }
        
        return Common::PrinterStatusUndefine;
    }

    /* 根据相应的类型解析状态 */
    switch(messageType)
    {
        case MessageTypeDevice:
            status = parseDeviceInfo(str);
            break;

        case MessageTypeJob:
            status = parseJobInfo(str);
            break;

        case MessageTypePage:
            status = parsePageInfo(str);
            break;

        case MessageTypeTimed:
            status = parseTimedInfo(str);
            break;

        default:
            status = Common::PrinterStatusUndefine;
            break;
    }

    return status;
}
