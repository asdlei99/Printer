#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include <net/WifiManage.h>

/*
 * 实现之前的所有功能
 * 提供槽函数，发射信号
 */

class WifiManager : public QObject
{
    Q_OBJECT
public:
    explicit WifiManager(QObject *parent = 0);
    ~WifiManager();

    void setNetworkCard(QString devName);

    bool scanWifi(std::vector<WifiManage::WifiInfo> &wifiInfo);

    bool connect(QString ssid, QString psk);

    bool disconnect();

    bool status(QString &wifi);

signals:

    void sigCntResult(bool result, QString wifi);

public slots:

    void slotCntWifi(QString ssid, QString psk);

private:

    WifiManage *wifiManage;

};

#endif // WIFIMANAGER_H
