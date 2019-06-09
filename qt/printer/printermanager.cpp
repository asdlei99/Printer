#include "printermanager.h"

PrinterManager::PrinterManager(QObject *parent) :
    QObject(parent)
{
    PrinterManage::getInstance()->registerCallBack(this);
}

PrinterManager::~PrinterManager()
{

}

void *PrinterManager::callBackFunc(void *arg)
{
    Printer::PrintInfo *printInfo = (Printer::PrintInfo *)arg;
    if(printInfo->printStatus == Printer::PrintStatusStart)
    {
        emit sigPrintStart(QString::fromStdString(printInfo->printTask.oldFileName));
    }
    else if(printInfo->printStatus == Printer::PrintStatusFinish)
    {
        emit sigPrintFinish(QString::fromStdString(printInfo->printTask.oldFileName));
    }

    return NULL;
}

bool PrinterManager::getSupportPrinter(std::vector<std::string> &printerVector)
{
    return PrinterManage::getInstance()->getSupportPrinter(printerVector);
}

bool PrinterManager::selectPrinter(QString devName, QString devPath)
{
    if(PrinterManage::getInstance()->isSelectDev() == true)
        PrinterManage::getInstance()->cancelPrinter();

    return PrinterManage::getInstance()->selectPrinter(devName.toStdString(), devPath.toStdString());
}

bool PrinterManager::cancelePrinter()
{
    if(PrinterManage::getInstance()->isSelectDev() == true)
        PrinterManage::getInstance()->cancelPrinter();

    return true;
}

bool PrinterManager::printerInit()
{
    return PrinterManage::getInstance()->printerInit();
}

bool PrinterManager::printerStartWork()
{
    return PrinterManage::getInstance()->printerStartWork();
}

bool PrinterManager::printerStopWork()
{
    return PrinterManage::getInstance()->printerStopWork();
}

bool PrinterManager::addTask(Printer::PrintTask &printTask)
{
    return PrinterManage::getInstance()->addTask(printTask);
}

bool PrinterManager::delTask(Printer::PrintTask &printTask)
{
    return PrinterManage::getInstance()->delTask(printTask);
}
