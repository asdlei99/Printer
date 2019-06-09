#include <print/Printer.h>
#include <lib/Parse.h>
#include <log/Log.h>
#include <unistd.h>
#include <algorithm>

Printer::Printer()
{
    devPath.assign("/dev/usb/lp0");

    printerDrv = NULL;

    pdfDeal = new PdfDeal;

    imageDeal = new ImageDeal;

    buf = new char[200];

    taskNum = 0;

    sem_init(&fileDealSem, 0, 0); // 文件处理线程使用

    sem_init(&printSem, 0, 0); // 打印线程使用

    srand((unsigned)time(NULL));
}

Printer::~Printer()
{
    printThreadRun = false;
    fileDealThreadRun = false;

    if(printerDrv)
    {
        delete printerDrv;
        printerDrv = NULL;
    }
    
    if(pdfDeal)
    {
        delete pdfDeal;
        pdfDeal = NULL;
    }

    if(imageDeal)
    {
        delete imageDeal;
        imageDeal = NULL;
    }

    if(buf)
    {
        delete buf;
        buf = NULL;
    }

    sem_destroy(&fileDealSem);

    sem_destroy(&printSem);
}

bool Printer::selectPrinter(std::string printerName)
{
    /* 设置打印机名字 */
    this->printerName = printerName;

    /* 设置相应的驱动 */
    if(Parse::printerFirmMap.find(this->printerName) != Parse::printerFirmMap.end())
    {
        if(printerDrv)
            delete printerDrv;

        printerDrv = new Foo2zjs;
    }
    else if(Parse::printerPpdMap.find(this->printerName) != Parse::printerPpdMap.end())
    {
        LOG_WARNING("Can't support this printer.\n");
        return false;
    }
    else
    {
        LOG_WARNING("Can't support this printer.\n");
        return false;
    }

    return true;
}

bool Printer::setDevPath(std::string devPath)
{
    this->devPath = devPath;

    return true;
}

std::string Printer::getDevName()
{
    return this->printerName;
}

bool Printer::selectStatusParser()
{
    /* 根据设备名，选择合适的状态解析器 */
    if(!strncmp(this->printerName.c_str(), "HP", 2))
    {
        if(!printerDrv)
        {
            LOG_ERROR("Please set printer driver.\n");
            return false;
        }

        printerDrv->setStatusParser(new HpStatusParser);
    }
    else
    {
        LOG_ERROR("Can't set status parser.\n");
        return false;
    }

    return true;
}

bool Printer::printerInit()
{
    if(!printerDrv)
    {
        LOG_ERROR("Can't find priner driver.\n");
        return false;
    }

    /* 设置驱动 */
    printerDrv->selectDev(this->printerName);
    printerDrv->setDevPath(this->devPath);

    /* 选择状态解析器 */
    selectStatusParser();

    /* 设备初始化 */
    printerDrv->devInit();

    return true;
}

bool Printer::printerExit()
{
    if(printThreadRun || fileDealThreadRun)
        stopWork();

    printThreadRun = false;
    fileDealThreadRun = false;

    return true;
}

bool Printer::printFile(Printer::PrintTask &printTask)
{
    strcpy(buf, printTask.fileName.c_str());

    /* 根据不同的驱动类型打印文件 */
    if(printerDrv->getDrvType() == PrinterDrv::DrvTypeFoo2zjs)
    {
        if(FileType::isPdf(buf))
        {
            /* 将pdf转换为pbm格式 */
            pdfDeal->deal2Pbm(printTask.fileName.c_str(), FileType::modifyName2Pbm(buf));
        }
        else if(FileType::isBmp(buf) || FileType::isPng(buf) || FileType::isJpg(buf))
        {
            /* 将图片转换为pbm格式 */
            imageDeal->deal2Pbm(printTask.fileName.c_str(), FileType::modifyName2Pbm(buf));
        }
        else
        {
            return false;
        }

        /* 打印文件 */
        printerDrv->printFile(buf, printTask.printParm);
    }
    else if(printerDrv->getDrvType() == PrinterDrv::DrvTypeHplip)
    {
        LOG_ERROR("Can't find driver.\n");
        return false;
    }
    else
    {
        LOG_ERROR("Can't find driver.\n");
        return false;
    }

    return true;
}

bool Printer::startWork()
{
    /* 开启文件处理线程、打印任务处理线程 */
    fileDealThreadRun = true;
    printThreadRun = true;
    fileDealThread.start(this);
    printThread.start(this);

    return true;
}

bool Printer::stopWork()
{
    if(fileDealThreadRun == true)
    {
        fileDealThreadRun = false;
        fileDealThread.stop();
    }

    if(printThreadRun == true)    
    {
        printThreadRun = false;
        printThread.stop();
    }

    return true;
}
bool Printer::addTask(PrintTask &printTask)
{
    unprocessedTask.push_back(printTask);
    taskNum++;

    sem_post(&fileDealSem);

    return true;
}

bool Printer::deleteTask(PrintTask &printTask)
{
    /* 从任务链表中删除打印任务 */
    std::list<PrintTask>::iterator it = find(unprocessedTask.begin(), unprocessedTask.end(), printTask);
    if(it != unprocessedTask.end())
    {
        LOG_DEBUG("Del task.\n");
        unprocessedTask.erase(it);
        taskNum--;
        return true;
    }

    it = find(processedTask.begin(), processedTask.end(), printTask);
    if(it != processedTask.end())
    {
        LOG_DEBUG("Del task.\n");
        processedTask.erase(it);
        taskNum--;
        return true;
    }

    LOG_DEBUG("No such task.\n");
    
    return false;
}

bool Printer::deleteTask(int taskSign)
{
    for(std::list<PrintTask>::iterator it = unprocessedTask.begin(); it != unprocessedTask.end(); it++)
    {
        if(it->taskSign == taskSign)
        {
            unprocessedTask.erase(it);
            return true;
        }
    }

    for(std::list<PrintTask>::iterator it = processedTask.begin(); it != processedTask.end(); it++)
    {
        if(it->taskSign == taskSign)
        {
            processedTask.erase(it);
            return true;
        }
    }

    return false;
}

int Printer::getTaskNum()
{
    return taskNum;
}

#if 0
bool Printer::registerCallBack(CallBack *callBack)
{
    callBackList.push_back(callBack);

    return true;
}

bool Printer::unregisterCallBack(CallBack *callBack)
{
    for(std::list<CallBack *>::iterator it = callBackList.begin(); it != callBackList.end(); it++)
    {
        if((*it) == callBack)
        {
            callBackList.erase(it);
            return true;
        }
    }

    return false;
}
#endif

bool Printer::registerCallBackTask(Common::CallBackTask &callBackTask)
{
    callBackTaskList.push_back(callBackTask);

    return true;
}

bool Printer::unregisterCallBackTask(Common::CallBackTask &callBackTask)
{
    std::list<Common::CallBackTask>::iterator it = find(callBackTaskList.begin(), callBackTaskList.end(), callBackTask);
    if(it != callBackTaskList.end())
    {
        callBackTaskList.erase(it);
        return true;
    }

    return false;
}

bool Printer::unregisterCallBackTask(int taskSign)
{
    std::list<Common::CallBackTask>::iterator it;
    for(it = callBackTaskList.begin(); it != callBackTaskList.end(); it++)
    {
        if(it->taskSign == taskSign)
        {
            callBackTaskList.erase(it);
            return true;
        }
    }

    return false;
}

void Printer::FileDealThread::run(void *arg)
{
    Printer *printer = (Printer *)arg;
    PrinterDrv::DrvType drvType;
    char buf1[200];
    char buf2[200];
    PrintTask printTask;

    /* 获得对应的驱动类型 */
    drvType = printer->printerDrv->getDrvType();

    while(printer->fileDealThreadRun)
    {
        sem_wait(&printer->fileDealSem);

        if(printer->unprocessedTask.empty())
        {
            LOG_INFO("UnprocessedTask empty.\n");
            sleep(1);
            continue;
        }

        if(drvType == PrinterDrv::DrvTypeFoo2zjs)
        {
            printTask = printer->unprocessedTask.front();
            printTask.oldFileName = printTask.fileName;
            printer->unprocessedTask.pop_front();
            strcpy(buf1, printTask.fileName.c_str());

            strcpy(buf2, buf1);
            FileType::modifyName2Pbm(buf2);
            printTask.fileName = std::string(buf2);

            if(FileType::isPdf(buf1))
            {
                printer->pdfDeal->deal2Pbm(buf1, buf2);

                printer->processedTask.push_back(printTask);

                sem_post(&printer->printSem);
            }
            else if(FileType::isBmp(buf1) || FileType::isJpg(buf1) || FileType::isPng(buf1))
            {
                printer->imageDeal->deal2Pbm(buf1, buf2);
                printer->processedTask.push_back(printTask);
            
                sem_post(&printer->printSem);
            }
            else
            {
                LOG_WARNING("Can't support this format file.\n");
            }
        }
        else if(drvType == PrinterDrv::DrvTypeHplip)
        {
            printTask = printer->unprocessedTask.front();
            printer->unprocessedTask.pop_front();
            strcpy(buf1, printTask.fileName.c_str());
            if(!FileType::isPdf(buf1))
                continue;

            printer->processedTask.push_back(printTask);

            sem_post(&printer->printSem);
        }
        else
        {
            LOG_ERROR("Can't support this driver type.\n");
            printer->fileDealThreadRun = false;
        }
    }

    printer->fileDealThreadRun = false;
}

void Printer::PrintThread::run(void *arg)
{
    Printer *printer = (Printer *)arg;
    PrintTask printTask;
    PrintInfo printInfo;

    while(printer->printThreadRun)
    {
        sem_wait(&printer->printSem);

        if(printer->processedTask.empty())
        {
            LOG_INFO("ProcessedTask empty.\n");
            sleep(1);
            continue;
        }

        printTask = printer->processedTask.front();
        printer->processedTask.pop_front();

        printInfo.printTask = printTask;
        printInfo.printStatus = PrintStatusStart;

#if 0
        /* 打印前调用回调函数 */
        for(std::list<CallBack *>::iterator it = printer->callBackList.begin(); it != printer->callBackList.end(); it++)
            (*it)->callBackFunc(&printInfo);
#endif

        for(std::list<Common::CallBackTask>::iterator it = printer->callBackTaskList.begin();
                                                    it != printer->callBackTaskList.end(); it++)
            it->func(arg, &printInfo);

        printer->printerDrv->printFile(printTask.fileName.c_str(), printTask.printParm);
        printer->taskNum--;

        printer->printerDrv->waitPrintEnd();

        printInfo.printStatus = PrintStatusFinish;

#if 0
        /* 打印后调用回调函数 */
        for(std::list<CallBack *>::iterator it = printer->callBackList.begin(); it != printer->callBackList.end(); it++)
            (*it)->callBackFunc(&printInfo);
#endif

        for(std::list<Common::CallBackTask>::iterator it = printer->callBackTaskList.begin();
                                                        it != printer->callBackTaskList.end(); it++)
            it->func(arg, &printInfo);
    }

    printer->printThreadRun = false;
}
