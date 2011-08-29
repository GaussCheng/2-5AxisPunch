#include <QSettings>
#include "icsystemconfig.h"

ICSystemConfig::ICSystemConfig(QObject *parent) :
    QObject(parent),
    CommunicationConfigGroup("CommunicationConfig"),
    MoldNameConfigGroup("MoldNameConfig")
{
    sysSetting_ = new QSettings("SZHC", "Multi-axisManipulatorSystem");
}

ICSerialPortConfig ICSystemConfig::SerialPortConfig() const
{
    ICSerialPortConfig config;
    sysSetting_->beginGroup(CommunicationConfigGroup);
    config.SetDevice(sysSetting_->value("Device", "/dev/ttyS0").toString());
    config.SetBaudRate(sysSetting_->value("BaudRate", 115200).toInt());
    config.SetParity(sysSetting_->value("Parity", "none").toString());
    config.SetDataBit(sysSetting_->value("DataBit", 8).toInt());
    config.SetStopBit(sysSetting_->value("StopBit", 2).toInt());
    sysSetting_->endGroup();
    return config;
}

void ICSystemConfig::SetSerialPortConfig(ICSerialPortConfig config)
{
    sysSetting_->beginGroup(CommunicationConfigGroup);
    sysSetting_->setValue("Device", config.DeviceName());
    sysSetting_->setValue("BaudRate", config.BaudRate());
    sysSetting_->setValue("Parity", config.Parity());
    sysSetting_->setValue("DataBit", config.DataBit());
    sysSetting_->setValue("StopBit", config.StopBit());
    sysSetting_->endGroup();
}

QString ICSystemConfig::MoldName() const
{
    sysSetting_->beginGroup(MoldNameConfigGroup);
    QString ret = sysSetting_->value("MoldName", "TEST").toString();
    sysSetting_->endGroup();
    return ret;
}

void ICSystemConfig::SetMoldName(const QString &name)
{
    sysSetting_->beginGroup(MoldNameConfigGroup);
    sysSetting_->setValue("MoldName", name);
    sysSetting_->endGroup();
}
