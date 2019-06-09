#include <print/PrinterManage.h>
#include <log/Log.h>
#include <algorithm>

PrinterManage *PrinterManage::printerManage = NULL;

PrinterManage::PrinterManage() :
    printer(new Printer),
    isSelectPrinter(false),
    isStartWork(false)
{

}

PrinterManage *PrinterManage::getInstance()
{
    if(!PrinterManage::printerManage)
    {
        PrinterManage::printerManage = new PrinterManage;
    }

    return PrinterManage::printerManage;
}

bool PrinterManage::getSupportPrinter(std::vector<std::string> &printerVector)
{
    printerVector = Parse::printer;

    return true;
}

bool PrinterManage::getCurWorkPrinter(std::string &printerName)
{
    if(isSelectPrinter == false)
        return false;

    printerName = this->printer->getDevName();

    return true;
}

bool PrinterManage::selectPrinter(std::string devName, std::string devPath)
{
    if(printer->selectPrinter(devName) == false)
    {
        LOG_WARNING("Select printer failure.\n");
        return false;
    }

    printer->setDevPath(devPath);
    
    isSelectPrinter = true;

    return true;
}

bool PrinterManage::cancelPrinter()
{
    if(isSelectPrinter == false)
        return false;

    printer->printerExit();

    isStartWork = false;
    isSelectPrinter = false;

    return true;
}

bool PrinterManage::printerInit()
{
    if(isSelectPrinter == false || isStartWork == true)
        return false;
    
    printer->printerInit();

    return true;
}

bool PrinterManage::printerStartWork()
{
    if(isSelectPrinter == false || isStartWork == true)
        return false;

    printer->startWork();

    isStartWork = true;

    return true;
}

bool PrinterManage::printerStopWork()
{
    if(isSelectPrinter == false || isStartWork == false)
        return false;

    printer->stopWork();
    isStartWork = false;

    return true;
}

bool PrinterManage::addTask(Printer::PrintTask &printTask)
{
    return printer->addTask(printTask);
}

bool PrinterManage::delTask(Printer::PrintTask &printTask)
{
    return printer->deleteTask(printTask);
}

bool PrinterManage::delTask(int taskSign)
{
    return printer->deleteTask(taskSign);
}

#if 0
bool PrinterManage::registerCallBack(CallBack *callBack)
{
   return  printer->registerCallBack(callBack);
}

bool PrinterManage::unregisterCallBack(CallBack *callBack)
{
    return printer->unregisterCallBack(callBack);
}
#endif

bool PrinterManage::registerCallBackTask(Common::CallBackTask &callBackTask)
{
    return printer->registerCallBackTask(callBackTask);
}

int PrinterManage::registerCallBackTask(void (*func)(void *, void *), void *arg)
{
    Common::CallBackTask task;

    task.func = func;
    task.arg = arg;

    if(printer->registerCallBackTask(task) == false)
    {
        LOG_DEBUG("Register callback failure.\n");
        return -1;
    }

    return task.taskSign;
}

bool PrinterManage::unregisterCallBackTask(Common::CallBackTask &callBackTask)
{
    return printer->unregisterCallBackTask(callBackTask);
}

bool PrinterManage::unregisterCallBackTask(int taskSign)
{
    return printer->unregisterCallBackTask(taskSign);
}

bool PrinterManage::isCanPrint()
{
    int num = printer->getTaskNum();

    if(num >= 100)
        return false;

    return true;
}

bool PrinterManage::isSelectDev()
{
    return isSelectPrinter;
}
