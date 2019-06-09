#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <print/PrinterDrv.h>
#include <log/Log.h>

PrinterDrv::PrinterDrv(PrinterDrv::DrvType drvType)
{
    this->drvType = drvType;
    devPath.assign("/dev/usb/lp0");
    buf = new char[200];
    printerStatusParser = NULL;
}

PrinterDrv::~PrinterDrv()
{
    if(printerStatusParser)
    {
        delete printerStatusParser;
        printerStatusParser = NULL;
    }

    if(buf)
    {
        delete[] buf;
        buf = NULL;
    }
}

bool PrinterDrv::selectDev(std::string devName)
{
    this->devName = devName;

    return true;
}

bool PrinterDrv::setDevPath(std::string devPath)
{
    this->devPath = devPath;

    return true;
}

bool PrinterDrv::setStatusParser(PrinterStatusParser *printerStatusParser)
{
    /* 如果当前有解析器，就更新解析器 */
    if(this->printerStatusParser)
        delete this->printerStatusParser;

    this->printerStatusParser = printerStatusParser;

    if(!printerStatusParser)
    {
        LOG_ERROR("Please set printer status parser.\n");
        return false;
    }

    return true;
}

PrinterDrv::DrvType PrinterDrv::getDrvType()
{
    return drvType;
}

bool PrinterDrv::waitPrintEnd()
{
    /* 打开打印机 */
    FILE *pFile = fopen(devPath.c_str(), "r");
    if(!pFile)
    {
        LOG_ERROR("Dev buzy.\n");
        return false;
    }

    /* 判断解析器是否为空 */
    if(!printerStatusParser)
    {
        LOG_WARNING("Please set printer status parser.\n");
        return false;
    }

    char *p1;
    Common::PrinterStatus status;

    /* 读取打印机返回数据，根据解析器判断参数意思 */
    while(1)
    {    
        p1 = fgets(buf, 1024, pFile);

        status = printerStatusParser->parse(p1);
        if(status == Common::PrinterStatusEnd)
            break;
    }
    
    fclose(pFile);

    return true;
}
