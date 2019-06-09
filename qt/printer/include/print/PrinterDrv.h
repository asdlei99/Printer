#ifndef _PRINTER_DRV_H_
#define _PRINTER_DRV_H_
#include <string>
#include <common/Common.h>
#include <print/PrinterStatusParser.h>
#include <pthread.h>

class PrinterDrv
{
public:

    enum DrvType
    {
        DrvTypeUndefine = -1,
        DrvTypeFoo2zjs,
        DrvTypeHplip,
    };

    struct PrintParm
    {
        PrintParm() : page(1) {}

        int page;

        bool operator = (const PrintParm &parm) {
            this->page = parm.page;
            return true;
        }
    };

    PrinterDrv(DrvType drvType);

    virtual ~PrinterDrv();

    /*
     * 函数名称： setStatusParser
     * 功能描述： 设置状态解析器，解析相应的打印机返回的数据
     * 输入参数： printerStatusParser - 状态解析器
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool setStatusParser(PrinterStatusParser *printerStatusParser);

    /*
     * 函数名称： selectDev
     * 功能描述： 选择打印机
     * 输入参数： devName - 打印机名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool selectDev(std::string devName);

    /*
     * 函数名称： setDevPath
     * 功能描述： 设置打印机对应的设备节点
     * 输入参数： devPath - 设备节点路径
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool setDevPath(std::string devPath);

    /*
     * 函数名称： waitPrintEnd
     * 功能描述： 等待打印任务完成
     * 输入参数： printerStatusParser - 状态解析器
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool waitPrintEnd();

    /*
     * 函数名称： getDrvType
     * 功能描述： 等待打印任务完成
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： 打印驱动的类型
     */
    DrvType getDrvType();

    /*
     * 函数名称： devInit
     * 功能描述： 打印机初始化函数
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    virtual bool devInit() = 0;

    /*
     * 函数名称： printFile
     * 功能描述： 打印文件
     * 输入参数： fileName - 打印文件绝对路径
     * 输入参数： printParm - 打印参数设置
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    virtual bool printFile(const char *fileName, PrintParm &printParm) = 0;

protected:

    DrvType drvType;

    std::string devName;

    std::string devPath;

    PrinterStatusParser *printerStatusParser;

    char *buf;
};

#endif //_PRINTER_DRV_H_