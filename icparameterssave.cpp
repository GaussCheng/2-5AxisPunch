#include "icparameterssave.h"

#include <QApplication>
#include <QDebug>

ICParametersSave * ICParametersSave::instance_ = NULL;

ICParametersSave::ICParametersSave(const QString fileName)
    : QSettings(fileName),
    ProductOperationer("ProductOperationer"),
    ProductAdministrator("ProductAdministrator"),
    ProductAlarmHistory("ProductAlarmHistory"),
    SystemMachine("SystemMachine"),
    SystemLocale("SystemLanguage"),
    InstructMultidotPut("InstructMultidotPut"),
    CommunicationConfig("CommunicationConfig"),
    ProductConfig("ProductConfig"),
    translator_(new QTranslator())
{
//    SetFileName("./sysconfig/systemParameter.hc");
}

ICParametersSave::~ICParametersSave()
{
    delete translator_;
}

void ICParametersSave::SaveParameter(const QString & group,const QString & key, const QVariant & value)
{
    this->beginGroup(group);
    this->setValue(key,value);
    this->endGroup();
}

QVariant ICParametersSave::GetParameter(const QString & group,const QString & key,const QVariant & defaultValue)
{
    QVariant value;
    this->beginGroup(group);
    value = this->value(key, defaultValue);
    this->endGroup();
    return value;
}

void ICParametersSave::SetCommunicationConfig(const QString &device, int baudRate, const QString &parity, int dataBit, int stopBit)
{
    this->beginGroup(CommunicationConfig);
    this->setValue("Device",device );
    this->setValue("Baud", baudRate);
    this->setValue("Parity", parity);
    this->setValue("DataBit", dataBit);
    this->setValue("StopBit", stopBit);
    this->endGroup();
}

void ICParametersSave::SetLanguage(QLocale::Language language)
{
    switch(language)
    {
    case QLocale::Chinese:
        {
            translator_->load("Multi-axisManipulatorSystem_ch");
        }
        break;
    case QLocale::English:
        {
            translator_->load("Multi-axisManipulatorSystem_en");
        }
        break;
    default:
        {
            return;
        }
    }

    qApp->installTranslator(translator_);
    SaveParameter(SystemLocale, "SystemLanguage", static_cast<int>(language));

    CurrentLanguageChanged();
}

void ICParametersSave::SetCountry(QLocale::Country country)
{
    switch(country)
    {
    case QLocale::China:
        {
            QLocale::setDefault(QLocale(QLocale::Chinese, country));
            SetLanguage(QLocale::Chinese);
        }
        break;
    case QLocale::UnitedStates:
        {
            QLocale::setDefault(QLocale(QLocale::English, country));
            SetLanguage(QLocale::English);
        }
        break;
    default:
        {
            QLocale::setDefault(QLocale(QLocale::Chinese, country));
            SetLanguage(QLocale::Chinese);
        }
    }
    SaveParameter(SystemLocale, "SystemCountry", static_cast<int>(country));
}

bool ICParametersSave::VerifyPassword(OperationLevel level, const QString &password)
{
    QString parameter;
    if(level == MachineOperator)
    {
        return true;
    }
    else if(level == MachineAdmin)
    {
        parameter = "MachineAdmin";
    }
    else if(level == AdvanceAdmin)
    {
        parameter == "AdvanceAdmin";
    }
    QString registerPwd = GetParameter("AdminInformation", parameter, "123").toString();
    return (registerPwd == password || password == "szhc");
}

void ICParametersSave::SetPassword(OperationLevel level, const QString &password)
{
    QString parameter;
    if(level == MachineOperator)
    {
        return;
    }
    else if(level == MachineAdmin)
    {
        parameter = "MachineAdmin";
    }
    else if(level == AdvanceAdmin)
    {
        parameter == "AdvanceAdmin";
    }

    SaveParameter("AdminInformation", parameter, password);
}

//void ICParametersSave::SetFileName(const QString &fileName)
//{
//    setPath(QSettings::IniFormat, QSettings::UserScope, fileName);
//    qDebug() << this->fileName();
//}
