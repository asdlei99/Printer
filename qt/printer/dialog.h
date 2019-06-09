#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <vector>
#include <QTimer>
#include <QThread>
#include <log/Log.h>
#include "inputdialog.h"
#include "wifimanager.h"
#include "printermanager.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void formInit();
    void uiInit();

signals:
    void sigStartCntWifi(QString ssid, QString psk);

private slots:
    void buttonClick();
    void slotBtnRenewClik();
    void slotBtnCntWifi();
    void slotInputDlgFinish();
    void slotWifiCntStat();
    void slotCntWifiResult(bool result, QString wifi);
    void slotBtnDiscntWifi();
    void slotBtnSelectPrinter();
    void slotBtnCancelPrinter();
    void slotPrintStart(QString file);
    void slotPrintFinish(QString file);

private:
    Ui::Dialog *ui;

    void connectInit();

    QString labelQssRed;
    QString labelQssGreen;
    QString labelQssYellow;

    InputDialog *inputDialog;

    WifiManager *wifiManager;
    std::vector<WifiManage::WifiInfo> wifiInfo;
    QString ssid;
    QString psk;
    QTimer *wifiCntTimer;
    QThread wifiWorkTh;

    PrinterManager *printerManager;
    QString printerName;
    bool printerStatus;
};

#endif // DIALOG_H
