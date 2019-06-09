#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#include <QObject>
#include <print/PrinterManage.h>

class PrinterManager : public QObject, public CallBack
{
    Q_OBJECT
public:
    explicit PrinterManager(QObject *parent = 0);
    ~PrinterManager();

    bool getSupportPrinter(std::vector<std::string> &printerVector);

    bool selectPrinter(QString devName, QString devPath);

    bool cancelePrinter();

    bool printerInit();

    bool printerStartWork();

    bool printerStopWork();

    bool addTask(Printer::PrintTask &printTask);

    bool delTask(Printer::PrintTask &printTask);

    virtual void *callBackFunc(void *arg);

signals:
    void sigPrintStart(QString fileName);
    void sigPrintFinish(QString fileName);

};

#endif // PRINTERMANAGER_H
