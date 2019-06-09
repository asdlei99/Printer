#include "Main.h"

#if 0
int test01()
{
    Printer *printer = new Printer;

    printer->selectPrinter("HP1020");
    printer->setDevPath("/dev/usb/lp0");
    printer->printerInit();

    printer->startWork();

    Printer::PrintTask prinTask1;
    LOG_DEBUG("Task sign:%d.\n", prinTask1.taskSign);
    prinTask1.fileName = std::string("./file/v1.png");
    prinTask1.printParm.page = 1;
    printer->addTask(prinTask1);

    Printer::PrintTask prinTask2;
    LOG_DEBUG("Task sign:%d.\n", prinTask2.taskSign);
    prinTask2.fileName = std::string("./file/v1.png");
    prinTask2.printParm.page = 2;
    printer->addTask(prinTask2);

    Printer::PrintTask prinTask3;
    LOG_DEBUG("Task sign:%d.\n", prinTask3.taskSign);
    prinTask3.fileName = std::string("./file/v1.png");
    prinTask3.printParm.page = 2;
    printer->addTask(prinTask3);

    if(printer->deleteTask(prinTask3) == true)
    {
        LOG_DEBUG("Del sucessed.\n");
    }

    if(printer->deleteTask(prinTask2) == true)
    {
        LOG_DEBUG("Del sucessed.\n");
    }

    int taskNum;
    while(1)
    {
        taskNum = printer->getTaskNum();
        printf("------------------------%d---------------------\n", taskNum);
        if(taskNum <= 0)
        {
            LOG_DEBUG("Finish.\n");
            break;
        }

        sleep(1);
    }

    return 0;
}

int test02()
{
    PrinterDrv *printerDrv = new Foo2zjs;
    FileDeal *fileDeal = new ImageDeal;

    printerDrv->selectDev("HP1020");
    printerDrv->setDevPath("/dev/usb/lp0");
    printerDrv->setStatusParser(new HpStatusParser);
    printerDrv->devInit();

    fileDeal->deal2Pbm("./file/v1.png", "./file/v1.pbm");

    PrinterDrv::PrintParm printParm;
    printParm.page = 2;
    printerDrv->printFile("./file/v1.pbm", printParm);

    while(1)
    {
        sleep(1);
    }

    return 0;
}

class CallBack1 : public CallBack
{
public:

    virtual void *callBackFunc(void *arg) {
        Printer::PrintTask *printTask = (Printer::PrintTask *)arg;

        std::cout<<"filename"<<printTask->fileName<<std::endl;
        std::cout<<"page"<<printTask->printParm.page<<std::endl;

        return NULL;
    };
};

class CallBack2 : public CallBack
{
public:

    virtual void *callBackFunc(void *arg) {
        Printer::PrintTask *printTask = (Printer::PrintTask *)arg;

        std::cout<<"filename:"<<printTask->fileName<<std::endl;
        std::cout<<"page:"<<printTask->printParm.page<<std::endl;

        return NULL;
    };

};

int test03()
{
    PrinterManage *printerManage = PrinterManage::getInstance();
    std::vector<std::string> printerVector;
    CallBack *callBack1 = new CallBack1;
    CallBack *callBack2 = new CallBack2;

    printerManage->getSupportPrinter(printerVector);

    for(std::vector<std::string>::iterator it = printerVector.begin(); it != printerVector.end(); it++)
    {
        std::cout<<*it<<std::endl;
    }

    printerManage->addPrinter("HP1020", "/dev/usb/lp0");

    printerManage->setDefaultPrinter("HP1020");

    printerManage->printerInit("HP1020");

    printerManage->registerCallBack("HP1020", callBack1);

    printerManage->registerCallBack("HP1020", callBack2);

    printerManage->printerStartWork("HP1020");

    Printer::PrintTask task1;
    task1.fileName = std::string("./file/test.pdf");
    printerManage->addTask(task1);

    Printer::PrintTask task2;
    task2.fileName = std::string("./file/v3.jpg");
    task2.printParm.page = 2;
    printerManage->addTask(task2);

    while(1)
    {
        sleep(1);
    }

    return 0;
}

int test04()
{
    WifiManage *wifiMange = new WifiManage;
    std::vector<WifiManage::WifiInfo> wifiInfoVector;

    wifiMange->devInit("wlan0");

    wifiMange->scanWifi(wifiInfoVector);

    for(std::vector<WifiManage::WifiInfo>::iterator it = wifiInfoVector.begin(); it != wifiInfoVector.end(); it++)
        printf("%s\n", it->wifiName.c_str());

    wifiMange->connectWifi("98k", "123456789001");

    std::string wifiName;
    int num = 0;
    while(1)
    {
        sleep(1);

        if(wifiMange->isConnect(wifiName) == true)
        {
            LOG_DEBUG("Connect to %s.\n", wifiName.c_str());
            break;
        }

        if(num++ >= 60)
        {
            LOG_DEBUG("Connect fail.\n");
            wifiMange->disconnectWifi("98k");
            break;
        }

        LOG_DEBUG("Wait.\n");
    }

    wifiMange->devExit();

    delete wifiMange;

    return 0;
}

int test05()
{
    PrinterManage *printerManage = PrinterManage::getInstance();
    Client2 client;
    char *buf = new char[1024];

    printerManage->addPrinter("HP1020", "/dev/usb/lp0");

    printerManage->setDefaultPrinter("HP1020");

    printerManage->printerInit("HP1020");

    printerManage->printerStartWork("HP1020");

    client.setIpAndPort("192.168.31.115", 8888);

    while(1)
    {
        fgets(buf, 1024, stdin);
        if(!strcmp(buf, "connect\n"))
        {
            if(client.connectServer())
            {
                LOG_DEBUG("Connect success.\n");
                client.interact();
            }
        }
        else if(!strcmp(buf, "disconnect\n"))
        {
            if(client.disconnect())
            {
                LOG_DEBUG("Disconnect success.\n");
            }
        }       
    }

    delete buf;

    return 0;
}

int test06()
{
    Client2 client;
    char *buf = new char[1024];

    client.setIpAndPort("123.207.20.167", 8888);

    while(1)
    {
        fgets(buf, 1024, stdin);
        if(!strcmp(buf, "connect\n"))
        {
            if(client.connectServer())
            {
                LOG_DEBUG("Connect success.\n");
                client.interact();
            }
        }
        else if(!strcmp(buf, "disconnect\n"))
        {
            if(client.disconnect())
            {
                LOG_DEBUG("Disconnect success.\n");
            }
        }       
    }

    delete buf;

    return 0;
}

#endif

#if 0
class CallBack1 : public CallBack
{
public:

    virtual void *callBackFunc(void *arg) {

        LOG_DEBUG("Callback 1.\n");        
        Printer::PrintInfo *printInfo = (Printer::PrintInfo *)arg;
        std::cout<<"filename:"<<printInfo->printTask.oldFileName<<std::endl;
        std::cout<<"page:"<<printInfo->printTask.printParm.page<<std::endl;

        return NULL;
    };
};

class CallBack2 : public CallBack
{
public:

    virtual void *callBackFunc(void *arg) {

        LOG_DEBUG("Callback 2.\n");
        Printer::PrintInfo *printInfo = (Printer::PrintInfo *)arg;
        std::cout<<"filename:"<<printInfo->printTask.oldFileName<<std::endl;
        std::cout<<"page:"<<printInfo->printTask.printParm.page<<std::endl;

        return NULL;
    };

};
#endif

#if 0
void test01()
{
    PrinterManage *printerManage = PrinterManage::getInstance();
    std::vector<std::string> printerVector;
    CallBack *callBack1 = new CallBack1;
    CallBack *callBack2 = new CallBack2;

    printerManage->getSupportPrinter(printerVector);

    for(std::vector<std::string>::iterator it = printerVector.begin(); it != printerVector.end(); it++)
    {
        std::cout<<*it<<std::endl;
    }

    printerManage->addPrinter("HP1020", "/dev/usb/lp0");

    printerManage->setDefaultPrinter("HP1020");

    printerManage->printerInit("HP1020");

    printerManage->registerCallBack("HP1020", callBack1);

    printerManage->registerCallBack("HP1020", callBack2);

    printerManage->printerStartWork("HP1020");

    Printer::PrintTask task1;
    task1.fileName = std::string("/usr/local/file/test.pdf");
    printerManage->addTask(task1);

    Printer::PrintTask task2;
    task2.fileName = std::string("/usr/local/file/v1.png");
    printerManage->addTask(task2);

    while(1)
    {
        sleep(1);
    }
}
#endif

#if 0
void test01()
{
    PrinterManage *printerManage = PrinterManage::getInstance();
    std::vector<std::string> printerVector;
    CallBack *callBack1 = new CallBack1;
    CallBack *callBack2 = new CallBack2;

    printerManage->selectPrinter("HP1020", "/dev/usb/lp0");
    printerManage->printerInit();

    printerManage->registerCallBack(callBack1);
    printerManage->registerCallBack(callBack2);
    printerManage->printerStartWork();

    Printer::PrintTask task1;
    task1.fileName = std::string("/usr/local/file/test.pdf");
    printerManage->addTask(task1);

    Printer::PrintTask task2;
    task2.fileName = std::string("/usr/local/file/v1.png");
    printerManage->addTask(task2);

    while(1)
    {
        sleep(1);
    }
}
#endif

void test02()
{
    Client *client = new Client1;

    client->setIpAndPort("193.112.98.9", 2487);

    client->clientConnect();

    while(1)
    {
        sleep(10);
    }

    client->clientDisconnect();

}

int main(int argc, char *argv[])
{
    Parse::parseConfigFile();

    Log::getInstance().initLog(NULL, Log::LogDebug);
    //Log::getInstance().initLog(Parse::logPath, Log::LogDebug);

    //test01();

    test02();

    return 0;
}
