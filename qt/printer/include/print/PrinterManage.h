#ifndef _PRINTERMANAGE_H_
#define _PRINTERMANAGE_H_
#include <string>
#include <vector>
#include <print/Printer.h>
#include <lib/Parse.h>
#include <lib/CallBack.h>

class PrinterManage
{
public:

    /*
     * 函数名称： getInstance
     * 功能描述： 获取单例模式实例
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： 实例地址
     */    
    static PrinterManage *getInstance();

    /*
     * 函数名称： getSupportPrinter
     * 功能描述： 获取支持的设备
     * 输入参数： 无
     * 输出参数： printerVector - 支持设备的列表
     * 返 回 值： true - 成功；false - 失败
     */  
    bool getSupportPrinter(std::vector<std::string> &printerVector);

    /*
     * 函数名称： getCurWorkPrinter
     * 功能描述： 获取当前正在工作的打印机
     * 输入参数： 无
     * 输出参数： printerVector - 打印机的列表
     * 返 回 值： true - 成功；false - 失败
     */  
    bool getCurWorkPrinter(std::string &devName);

    /*
     * 函数名称： addPrinter
     * 功能描述： 添加打印机
     * 输入参数： devName - 设备名字
     * 输入参数： devPath - 设备节点
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */  
    bool selectPrinter(std::string devName, std::string devPath);

    /*
     * 函数名称： delPrinter
     * 功能描述： 删除打印机
     * 输入参数： devName - 设备名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */  
    bool cancelPrinter();

    /*
     * 函数名称： printerInit
     * 功能描述： 打印机设备初始化
     * 输入参数： devName - 设备名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */  
    bool printerInit();

    /*
     * 函数名称： printerStartWork
     * 功能描述： 打印机开始工作
     * 输入参数： devName - 设备名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */  
    bool printerStartWork();

    /*
     * 函数名称： printerStopWork
     * 功能描述： 打印机停止工作
     * 输入参数： devName - 设备名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */  
    bool printerStopWork();

    /*
     * 函数名称： delTask
     * 功能描述： 给指定打印机添加打印任务
     * 输入参数： devName - 设备名字
     * 输入参数： printTask - 打印任务
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool addTask(Printer::PrintTask &printTask);

    /*
     * 函数名称： delTask
     * 功能描述： 给指定打印机删除打印任务
     * 输入参数： devName - 设备名字
     * 输入参数： printTask - 打印任务
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool delTask(Printer::PrintTask &printTask);

    /*
     * 函数名称： delTask
     * 功能描述： 给指定打印机删除打印任务
     * 输入参数： devName - 设备名字
     * 输入参数： taskSign - 打印任务标识
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */ 
    bool delTask(int taskSign);

#if 0
    /*
     * 函数名称： registerCallBack
     * 功能描述： 给指定打印机注册回调函数，打印前后会调用
     * 输入参数： devName - 设备名字
     * 输入参数： callBack - 回调函数
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */     
    bool registerCallBack(CallBack *callBack);

    /*
     * 函数名称： unregisterCallBack
     * 功能描述： 给指定打印机注销回调函数
     * 输入参数： devName - 设备名字
     * 输入参数： callBack - 回调函数
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool unregisterCallBack(CallBack *callBack);    
#endif

    bool registerCallBackTask(Common::CallBackTask &callBackTask);

    int registerCallBackTask(void (*func)(void *, void *), void *);

    bool unregisterCallBackTask(Common::CallBackTask &callBackTask);

    bool unregisterCallBackTask(int taskSign);

    /*
     * 函数名称： isCanPrint
     * 功能描述： 判断当前是否可以打印
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool isCanPrint();

    bool isSelectDev();

private:

    PrinterManage();

    Printer *printer;

    bool isSelectPrinter;

    bool isStartWork;

    static PrinterManage *printerManage;
};

#endif //_PRINTERMANAGE_H_