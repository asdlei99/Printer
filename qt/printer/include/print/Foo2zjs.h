#ifndef _FOO2ZJS_H_
#define _FOO2ZJS_H_
#include <string>
#include <print/PrinterDrv.h>
#include <lib/Thread.h>

class Foo2zjs : public PrinterDrv
{
public:

    Foo2zjs();

    ~Foo2zjs();

    /*
     * 函数名称： devInit
     * 功能描述： 打印机初始化函数
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    virtual bool devInit();

    /*
     * 函数名称： printFile
     * 功能描述： 打印文件
     * 输入参数： fileName - 打印文件绝对路径，只支持pbm格式
     * 输入参数： printParm - 打印参数
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    virtual bool printFile(const char *fileName, PrintParm &printParm);

private:

    /*
     * 函数名称： burnFirmware
     * 功能描述： 给对应的打印机烧写固件
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool burnFirmware();

    std::string firmware;

    char *buf;

    char *cmd;

    const char *foo2zjs;

};

#endif //_FOO2ZJS_H_