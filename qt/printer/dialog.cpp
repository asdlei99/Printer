#include "dialog.h"
#include "ui_dialog.h"
#include "iconhelper.h"
#include "keyBoard.h"
#include <log/Log.h>
#include <QMessageBox>
#include <QString>
#include <unistd.h>
#include <string.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    wifiManager(new WifiManager),
    inputDialog(new InputDialog),
    wifiCntTimer(new QTimer(this)),
    printerManager(new PrinterManager),
    printerStatus(false)
{
    ui->setupUi(this);

    this->resize(1024, 600);
    wifiManager->setNetworkCard("wlan0");

    this->formInit();
    this->connectInit();
    this->uiInit();

    wifiManager->moveToThread(&wifiWorkTh);
    wifiWorkTh.start();
}

Dialog::~Dialog()
{
    wifiWorkTh.quit();
    wifiWorkTh.wait();

    delete ui;
    delete wifiCntTimer;
    delete inputDialog;
    delete wifiManager;
}

void Dialog::uiInit()
{
    QString wifi;
    if(wifiManager->status(wifi))
    {
        ui->label_wifiVal->setStyleSheet(labelQssGreen);
        ui->label_wifiVal->setText(QString("已连接\"%1\"").arg(wifi));
        ui->label_wifiStat->setStyleSheet(labelQssGreen);
        ui->label_wifiStat->setText(QString("已连接\"%1\"").arg(wifi));
        ssid = wifi;
    }

    wifiManager->scanWifi(wifiInfo);
    ui->listWidget_wifi->clear();
    for(std::vector<WifiManage::WifiInfo>::iterator it = wifiInfo.begin(); it != wifiInfo.end(); it++)
    {
        ui->listWidget_wifi->addItem(QString::fromStdString(it->name));
    }

    std::vector<std::string> printer;
    printerManager->getSupportPrinter(printer);
    for(std::vector<std::string>::iterator it = printer.begin(); it != printer.end(); it++)
    {
        ui->listWidget_printer->addItem(QString::fromStdString(*it));
    }
}

void Dialog::formInit()
{
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    /* 设置当前显示界面 */
    ui->stackedWidget_main->setCurrentIndex(0);

    /* 设置顶行风格 */
    ui->widget_menu->setProperty("nav", "top");
    ui->widget_top->setProperty("form", "title");

    ui->label_title->setFont(QFont("Microsoft Yahei", 20));
    ui->label_title->setText("打印机管理平台");

    /* 设置按钮 */
    QList<QToolButton *> tbtns = ui->widget_menu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        btn->setCheckable(true); // 选中效果
        connect(btn, &QToolButton::clicked, this, &Dialog::buttonClick);
    }
    ui->toolButton_mainPage->setChecked(true); // 设置为被选中

    ui->label_printer->setFont(QFont("Microsoft Yahei", 10));
    ui->label_printer->setText("选择打印机");

    IconHelper::Instance()->setIcon(ui->label_titilIcon, QChar(0xf099), 30);
    IconHelper::Instance()->setIcon(ui->label_printerIcon, QChar(0xf108), 10);
    IconHelper::Instance()->setIcon(ui->label_transferMassageIcon, QChar(0xf256), 10);
    IconHelper::Instance()->setIcon(ui->label_wifiSelectIcon, QChar(0xf1eb), 10);
    IconHelper::Instance()->setIcon(ui->label_connectServerIcon, QChar(0xf233), 10);
    IconHelper::Instance()->setIcon(ui->label_sysSettingIcon, QChar(0xf185), 10);

    labelQssRed = QString("QLabel{color:rgb(%1);}").arg("255,94,94");
    labelQssGreen = QString("QLabel{color:rgb(%1);}").arg("128,255,0");
    labelQssYellow = QString("QLabel{color:rgb(%1);}").arg("255,255,80");

    ui->label_serverVal->setStyleSheet(labelQssRed);
    ui->label_wifiVal->setStyleSheet(labelQssRed);
    ui->label_printerVal->setStyleSheet(labelQssRed);
    ui->label_wifiStat->setStyleSheet(labelQssRed);
    ui->label_serverVal->setText("未连接");
    ui->label_wifiVal->setText("未连接");
    ui->label_printerVal->setText("未选择");
    ui->label_wifiStat->setText("未连接");

    QList<QLineEdit *> lineEdits = ui->stackedWidget_main->findChildren<QLineEdit *>();
    foreach (QLineEdit *lineEdit, lineEdits)
    {
        connect(lineEdit, &QLineEdit::selectionChanged, KeyBoard::getInstance(), &KeyBoard::slotLineEditSelect);
    }
}

void Dialog::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    QList<QToolButton *> tbtns = ui->widget_menu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        if (btn == b)
        {
            btn->setChecked(true);
        }
        else
        {
            btn->setChecked(false);
        }
    }

    if(name == "主界面")
    {
        ui->stackedWidget_main->setCurrentIndex(0);
    }
    else if(name == "设置")
    {
        ui->stackedWidget_main->setCurrentIndex(1);
    }
}

void Dialog::connectInit()
{
    connect(ui->toolButton_scanWifi, &QToolButton::clicked, this, &Dialog::slotBtnRenewClik);
    connect(ui->toolButton_connectWifi, &QToolButton::clicked, this, &Dialog::slotBtnCntWifi);
    connect(inputDialog, &InputDialog::finish, this, &Dialog::slotInputDlgFinish);
    connect(wifiCntTimer, &QTimer::timeout, this, &Dialog::slotWifiCntStat);
    connect(this, &Dialog::sigStartCntWifi, wifiManager, &WifiManager::slotCntWifi);
    connect(wifiManager, &WifiManager::sigCntResult, this, &Dialog::slotCntWifiResult);
    connect(ui->toolButton_disconnectWifi, &QToolButton::clicked, this, &Dialog::slotBtnDiscntWifi);
    connect(ui->toolButton_selectPrinter, &QToolButton::clicked, this, &Dialog::slotBtnSelectPrinter);
    connect(ui->toolButton_cancelPrinter, &QToolButton::clicked, this, &Dialog::slotBtnCancelPrinter);
    connect(printerManager, &PrinterManager::sigPrintStart, this, &Dialog::slotPrintStart);
    connect(printerManager, &PrinterManager::sigPrintFinish, this, &Dialog::slotPrintFinish);
}

void Dialog::slotBtnRenewClik()
{
    wifiManager->scanWifi(wifiInfo);
    ui->listWidget_wifi->clear();
    for(std::vector<WifiManage::WifiInfo>::iterator it = wifiInfo.begin(); it != wifiInfo.end(); it++)
    {
        ui->listWidget_wifi->addItem(QString::fromStdString(it->name));
    }
}

void Dialog::slotBtnCntWifi()
{
    QString wifi;

    QListWidgetItem *item = ui->listWidget_wifi->currentItem();
    if(!item)
        return;

    if(wifiManager->status(wifi) == true)
    {
        if(wifi == item->text())
        {
            QString str = QString("已连接\"%1\"").arg(wifi);
            QMessageBox::question(this, "wifi连接", str,
                                          QMessageBox::Yes);
            return;
        }

        QString str = QString("是否断开wifi连接\"%1\"").arg(wifi);
        int ret = QMessageBox::question(this, "wifi连接", str,
                              QMessageBox::Yes | QMessageBox::Cancel,
                              QMessageBox::Cancel);
        if(ret == QMessageBox::Yes)
        {
            if(wifiManager->disconnect() == false)
            {
                QMessageBox::question(this, "提示", "断开连接失败", QMessageBox::Yes);
                return;
            }

            ui->label_wifiVal->setStyleSheet(labelQssRed);
            ui->label_wifiVal->setText(QString("未连接"));
            ui->label_wifiStat->setStyleSheet(labelQssRed);
            ui->label_wifiStat->setText(QString("未连接"));
        }
        else
            return;
    }

    inputDialog->setLabel("密码：");
    inputDialog->setTitle(QString("连接\"%1\"").arg(item->text()));
    inputDialog->show();
}

void Dialog::slotInputDlgFinish()
{
    if(inputDialog->getOption() == true)
    {
        QListWidgetItem *item = ui->listWidget_wifi->currentItem();
        if(!item)
            return;

        ui->label_wifiVal->setStyleSheet(labelQssYellow);
        ui->label_wifiStat->setStyleSheet(labelQssYellow);
        wifiCntTimer->start(500);
        ssid = item->text();
        psk = inputDialog->getText();
        emit sigStartCntWifi(ssid, psk);
    }
}

void Dialog::slotCntWifiResult(bool result, QString wifi)
{
    wifiCntTimer->stop();
    if(result == true)
    {
        ui->label_wifiVal->setStyleSheet(labelQssGreen);
        ui->label_wifiVal->setText(QString("已连接\"%1\"").arg(wifi));
        ui->label_wifiStat->setStyleSheet(labelQssGreen);
        ui->label_wifiStat->setText(QString("已连接\"%1\"").arg(wifi));
    }
    else
    {
        ui->label_wifiVal->setStyleSheet(labelQssRed);
        ui->label_wifiVal->setText(QString("未连接"));
        ui->label_wifiStat->setStyleSheet(labelQssRed);
        ui->label_wifiStat->setText(QString("未连接"));
    }
}

void Dialog::slotWifiCntStat()
{
    static int i = 0;

    switch(i)
    {
        case 0:
            ui->label_wifiVal->setText(QString("正在连接\"%1\".").arg(ssid));
            ui->label_wifiStat->setText(QString("正在连接\"%1\".").arg(ssid));
        break;

        case 1:
            ui->label_wifiVal->setText(QString("正在连接\"%1\"..").arg(ssid));
            ui->label_wifiStat->setText(QString("正在连接\"%1\"..").arg(ssid));
        break;

        case 2:
            ui->label_wifiVal->setText(QString("正在连接\"%1\"...").arg(ssid));
            ui->label_wifiStat->setText(QString("正在连接\"%1\"...").arg(ssid));
        break;

        case 3:
            ui->label_wifiVal->setText(QString("正在连接\"%1\"....").arg(ssid));
            ui->label_wifiStat->setText(QString("正在连接\"%1\"....").arg(ssid));
        break;
    }

    i++;
    if(i >= 4)
        i = 0;
}

void Dialog::slotBtnDiscntWifi()
{
    QString wifi;

    if(wifiManager->status(wifi) == false)
    {
        QMessageBox::question(this, "提示", "未连接wifi", QMessageBox::Yes);

        ui->label_wifiVal->setStyleSheet(labelQssRed);
        ui->label_wifiVal->setText(QString("未连接"));
        ui->label_wifiStat->setStyleSheet(labelQssRed);
        ui->label_wifiStat->setText(QString("未连接"));
    }
    else
    {
        QString str = QString("是否断开wifi连接\"%1\"").arg(wifi);
        int ret = QMessageBox::question(this, "wifi连接", str,
                              QMessageBox::Yes | QMessageBox::Cancel,
                              QMessageBox::Cancel);
        if(ret == QMessageBox::Yes)
        {
            if(wifiManager->disconnect() == false)
            {
                QMessageBox::question(this, "提示", "断开wifi连接失败", QMessageBox::Yes);
                return;
            }

            ui->label_wifiVal->setStyleSheet(labelQssRed);
            ui->label_wifiVal->setText(QString("未连接"));
            ui->label_wifiStat->setStyleSheet(labelQssRed);
            ui->label_wifiStat->setText(QString("未连接"));
        }
    }
}

void Dialog::slotBtnSelectPrinter()
{
    QListWidgetItem *it = ui->listWidget_printer->currentItem();
    if(!it)
        return;

    printerName = it->text();
    QString str = QString("是否选择\"%1\"").arg(printerName);
    int ret = QMessageBox::question(this, "打印机选择", str,
                          QMessageBox::Yes | QMessageBox::Cancel,
                          QMessageBox::Cancel);
    if(ret != QMessageBox::Yes)
        return;

    ui->label_printerVal->setStyleSheet(labelQssGreen);
    ui->label_printerVal->setText(QString("已选择\"%1\"").arg(printerName));
    printerManager->selectPrinter(printerName, "/dev/usb/lp0");
    printerManager->printerInit();
    printerStatus = true;

    printerManager->printerStartWork();

}

void Dialog::slotBtnCancelPrinter()
{
    if(printerStatus == false)
    {
        QMessageBox::question(this, "提示", "未选择打印机", QMessageBox::Yes);
        return;
    }

    QString str = QString("是否取消选择\"%1\"").arg(printerName);
    int ret = QMessageBox::question(this, "打印机选择", str,
                          QMessageBox::Yes | QMessageBox::Cancel,
                          QMessageBox::Cancel);
    if(ret != QMessageBox::Yes)
        return;

    printerManager->cancelePrinter();

    ui->label_printerVal->setStyleSheet(labelQssRed);
    ui->label_printerVal->setText(QString("未选择"));

    printerStatus = false;
}

void Dialog::slotPrintStart(QString file)
{
    const char *p = strrchr(file.toStdString().c_str(), '/');
    ui->textBrowser_message->append(QString("开始打印\"%1\"").arg(QString(p+1)));
}

void Dialog::slotPrintFinish(QString file)
{
    const char *p = strrchr(file.toStdString().c_str(), '/');
    ui->textBrowser_message->append(QString("打印完成\"%1\"").arg(QString(p+1)));
}
