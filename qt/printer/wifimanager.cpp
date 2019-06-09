#include "wifimanager.h"

WifiManager::WifiManager(QObject *parent) :
    QObject(parent),
    wifiManage(new WifiManage)
{

}

WifiManager::~WifiManager()
{
    delete wifiManage;
}

void WifiManager::setNetworkCard(QString devName)
{
    wifiManage->setNetworkCard(devName.toStdString());
}

bool WifiManager::scanWifi(std::vector<WifiManage::WifiInfo> &wifiInfo)
{
    return wifiManage->scanWifi(wifiInfo);
}

bool WifiManager::connect(QString ssid, QString psk)
{
    return wifiManage->connect(ssid.toStdString().c_str(), psk.toStdString().c_str());
}

bool WifiManager::disconnect()
{
    return wifiManage->disconnect();
}

bool WifiManager::status(QString &wifi)
{
    std::string name;

    if(wifiManage->status(name) == false)
        return false;

    wifi = QString::fromStdString(name);

    return true;
}

void WifiManager::slotCntWifi(QString ssid, QString psk)
{
    bool result;

    result = this->connect(ssid, psk);

    emit sigCntResult(result, ssid);
}
