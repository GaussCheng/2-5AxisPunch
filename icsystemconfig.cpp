#include <QSettings>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
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

ICUserDefineConfigSPTR ICUserDefineConfig::instance_;

void ICUserDefineConfig::Init()
{
    const QString sysconfigPath = "./sysconfig/";
    const QString userDefineXPath = sysconfigPath + "user_define_x";
    const QString userDefineYPath = sysconfigPath + "user_define_y";
    const QString userMachineDefineXPath = sysconfigPath + "user_define_machine_x";
    const QString userMachineDefineYPath = sysconfigPath + "user_define_machine_y";
    const QString userPointPath = sysconfigPath + "user_define_points";
    const QString userActionPath = sysconfigPath + "user_define_io_actions";
    ReadIOInfos_(userDefineXPath, this->xInfos_);
    ReadIOInfos_(userDefineYPath, this->yInfos_);
    ReadIOInfos_(userMachineDefineXPath, this->euxInfos_);
    ReadIOInfos_(userMachineDefineYPath, this->euyInfos_);
    ReadPointInfos_(userPointPath);
    ReadActionInfos_(userActionPath);
    euxStrings_<<euxInfos_.value(2).GetLocaleName("zh")
              <<euxInfos_.value(3).GetLocaleName("zh")
             <<euxInfos_.value(7).GetLocaleName("zh")
            <<euxInfos_.value(6).GetLocaleName("zh")
           <<euxInfos_.value(14).GetLocaleName("zh")
          <<euxInfos_.value(13).GetLocaleName("zh");
    euyStrings_<<euyInfos_.value(0).GetLocaleName("zh")
              <<euyInfos_.value(6).GetLocaleName("zh")
             <<euyInfos_.value(1).GetLocaleName("zh")
            <<euyInfos_.value(5).GetLocaleName("zh")
           <<euyInfos_.value(8).GetLocaleName("zh")
          <<euyInfos_.value(14).GetLocaleName("zh");

}

void ICUserDefineConfig::ReadIOInfos_(const QString &path, UserIOInfos &infos)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream outStream(&file);
        outStream.setCodec("UTF-8");
        QString content = outStream.readAll();
        file.close();
        QStringList rows = content.split("\n", QString::SkipEmptyParts);
        QStringList items;
        const int rowSize = rows.size();
        ICUserIOInfo ioInfo;
        for(int i = 0; i != rowSize; ++i)
        {
            items = rows.at(i).split(",", QString::SkipEmptyParts);
            if(items.size() < 2)
            {
                return;
            }
            ioInfo.hardwarePoint = items.at(0).toInt();
            ioInfo.code = items.at(1);
            if(items.size() > 2)
                ioInfo.localeNameMap.insert("zh", items.at(2));
            if(items.size() > 3)
                ioInfo.localeNameMap.insert("en", items.at(3));
            infos.insert(ioInfo.hardwarePoint, ioInfo);
        }
    }
}

void ICUserDefineConfig::ReadPointInfos_(const QString &path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream outStream(&file);
        outStream.setCodec("UTF-8");
        QString content = outStream.readAll();
        file.close();
        QStringList rows = content.split("\n", QString::SkipEmptyParts);
        QStringList items;
        const int rowSize = rows.size();
        ICUserPointInfo info;
        for(int i = 0; i != rowSize; ++i)
        {
            items = rows.at(i).split(",", QString::SkipEmptyParts);
            if(items.size() < 1)
            {
                return;
            }
            if(items.size() > 1)
                info.localeNameMap.insert("zh", items.at(1));
            if(items.size() > 2)
                info.localeNameMap.insert("en", items.at(2));
            this->pointInfos_.insert(items.at(0).toInt(), info);
        }
    }
}

void ICUserDefineConfig::ReadActionInfos_(const QString &path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream outStream(&file);
        outStream.setCodec("UTF-8");
        QString content = outStream.readAll();
        file.close();
        QStringList rows = content.split("\n", QString::SkipEmptyParts);
        QStringList items;
        const int rowSize = rows.size();
        ICUserActionInfo info;
        for(int i = 0; i != rowSize; ++i)
        {
            items = rows.at(i).split(",", QString::SkipEmptyParts);
            if(items.size() < 4)
            {
                return;
            }
            info.type = items.at(1).toInt();
            info.pointNum = items.at(2).toInt();
            info.dir = items.at(3).toInt();
            if(items.size() > 4)
                info.localeNameMap.insert("zh", items.at(4));
            if(items.size() > 5)
                info.localeNameMap.insert("en", items.at(5));
            this->actionInfos_.insert(items.at(0).toInt(), info);
        }
    }
}

QList<ICUserActionInfo> ICUserDefineConfig::GetActionInfosByType(int type)
{
    QList<ICUserActionInfo> ret;
    ICUserDefineConfig::UserActionInfos::iterator p = actionInfos_.begin();
    while(p != actionInfos_.end())
    {
        if(p.value().type == type || type == -1)
        {
            ret.append(p.value());
        }
        ++p;
    }
    return ret;
}

QString ICUserDefineConfig::GetIOActionLocaleName(int type, int id , bool dir, const QString& languageName)
{
    UserActionInfos::iterator p = actionInfos_.begin();
    qDebug()<<type<<id<<dir;
    while(p != actionInfos_.end())
    {
        if(p.value().type == type && p.value().pointNum == id && p.value().dir == dir)
        {
            return p.value().GetLocaleName(languageName);
        }
        ++p;
    }
    return "";
//        if(!actionInfos_.contains(id)) return "";
//        return actionInfos_.value(id).GetLocaleName(languageName);
}
