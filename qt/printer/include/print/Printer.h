#ifndef _PRINTER_DRIVER_H_
#define _PRINTER_DRIVER_H_
#include <string>
#include <vector>
#include <list>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <print/FileDeal.h>
#include <print/ImageDeal.h>
#include <print/PdfDeal.h>
#include <print/PrinterDrv.h>
#include <print/Foo2zjs.h>
#include <lib/Parse.h>
#include <print/PrinterStatusParser.h>
#include <print/HpStatusParser.h>
#include <lib/Thread.h>
#include <lib/CallBack.h>
#include <common/Common.h>

class Printer
{
public:

    struct PrintTask
    {
        PrintTask() {
            taskSign = rand();
        }

        std::string fileName;
        std::string oldFileName;
        PrinterDrv::PrintParm printParm;
        int taskSign;

        bool operator == (const PrintTask &task) {
            return this->taskSign == task.taskSign;
        }

        bool operator = (const PrintTask &task) {
            this->fileName = task.fileName;
            this->printParm = task.printParm;
            this->oldFileName = task.oldFileName;

            return true;
        }
    };
    
    enum PrintStatus {
        PrintStatusStart,
        PrintStatusFinish,
    };

    struct PrintInfo {
        PrintStatus printStatus;
        PrintTask printTask;
    };

    class FileDealThread : public Thread
    {
        void run(void *arg);
    };

    class PrintThread : public Thread
    {
        void run(void *arg);
    };

    Printer();

    ~Printer();

    /*
     * 函数名称： selectPrinter
     * 功能描述： 选择打印机
     * 输入参数： printerName - 打印机名字
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool selectPrinter(std::string printerName);

    /*
     * 函数名称： setDevPath
     * 功能描述： 设置打印机设备结点
     * 输入参数： devPath - 设备结点路径
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool setDevPath(std::string devPath);

    /*
     * 函数名称： getDevName
     * 功能描述： 获得打印机的名字
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： 打印机的名字
     */
    std::string getDevName();

    /*
     * 函数名称： printerInit
     * 功能描述： 打印机初始化
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool printerInit();

    /*
     * 函数名称： printerExit
     * 功能描述： 打印机退出
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool printerExit();

    /*
     * 函数名称： startWork
     * 功能描述： 打印机开始工作，开始处理打印任务
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool startWork();

    /*
     * 函数名称： stopWork
     * 功能描述： 打印机停止工作
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool stopWork();

    /*
     * 函数名称： printFile
     * 功能描述： 直接操控打印机打印文件
     * 输入参数： printTask - 打印任务
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool printFile(Printer::PrintTask &printTask);

    /*
     * 函数名称： addTask
     * 功能描述： 给打印机添加打印任务
     * 输入参数： printTask - 打印任务
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool addTask(PrintTask &printTask);

    /*
     * 函数名称： deleteTask
     * 功能描述： 删除相应的打印任务
     * 输入参数： printTask - 打印任务
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool deleteTask(PrintTask &printTask);

    /*
     * 函数名称： deleteTask
     * 功能描述： 通过任务标识号删除相应的打印任务
     * 输入参数： taskSign - 任务标识
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool deleteTask(int taskSign);

    /*
     * 函数名称： getTaskNum
     * 功能描述： 获取打印任务数
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： 当前打印任务数
     */
    int getTaskNum();

#if 0
    /*
     * 函数名称： registerCallBack
     * 功能描述： 注册回调函数，每当打印完成后会调用
     * 输入参数： callBack - 回调函数
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool registerCallBack(CallBack *callBack);

    /*
     * 函数名称： registerCallBack
     * 功能描述： 注销回调函数
     * 输入参数： callBack - 注销回调函数
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool unregisterCallBack(CallBack *callBack);
#endif

    /*
     * 函数名称： registerCallBackTask
     * 功能描述： 注册回调任务，每当打印完成后会调用
     * 输入参数： callBack - 回调函数
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool registerCallBackTask(Common::CallBackTask &callBackTask);

    /*
     * 函数名称： unregisterCallBackTask
     * 功能描述： 注销回调任务
     * 输入参数： callBack - 注销回调函数
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool unregisterCallBackTask(Common::CallBackTask &callBackTask);

    bool unregisterCallBackTask(int taskSign);

private:

    /*
     * 函数名称： selectStatusParser
     * 功能描述： 选择状态解析器
     * 输入参数： 无
     * 输出参数： 无
     * 返 回 值： true - 成功；false - 失败
     */
    bool selectStatusParser();

    std::string printerName;

    std::string devPath;

    PrinterDrv *printerDrv;

    FileDeal *pdfDeal;

    FileDeal *imageDeal;

    char *buf;

    int taskNum;

    std::list<PrintTask> unprocessedTask;

    std::list<PrintTask> processedTask;

    FileDealThread fileDealThread;

    bool fileDealThreadRun;

    PrintThread printThread;

    bool printThreadRun;

    sem_t fileDealSem;

    sem_t printSem;

//    std::list<CallBack *> callBackList;

    std::list<Common::CallBackTask> callBackTaskList;
};

#endif //_PRINTER_DRIVER_H_