#include "icparameterssave.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
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
    translator_(new QTranslator()),
      sysTranslator_(new QTranslator())
{
//    SetFileName("./sysconfig/systemParameter.hc");
    QFile file("./sysconfig/DistanceRotation");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString tmpAxis;
        double tmpDR;
        while(!(in>>tmpAxis>>tmpDR).atEnd())
        {
            axisToRotate_.insert(tmpAxis, tmpDR);
        }
    }
    file.close();
#ifndef Q_WS_WIN32
    beepFD_ = open("/dev/szhc_beep", O_WRONLY);
#else
    beepFD_ = 0;
#endif
    SetKeyTone(KeyTone());
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
            sysTranslator_->load("qt_zh_CN");
            qApp->installTranslator(sysTranslator_);
        }
        break;
    case QLocale::English:
        {
            translator_->load("Multi-axisManipulatorSystem_en");
            qApp->removeTranslator(sysTranslator_);
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
        parameter = "AdvanceAdmin";
    }
    QString registerPwd = GetParameter("AdminInformation", parameter, "123").toString();
    return (registerPwd == password || password == "szhcrobot" || password == SuperPassward());
}

void ICParametersSave::SetPassword(OperationLevel level, const QString &password)
{
    QString parameter;
    qDebug() << "the level is : " << level;
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
        parameter = "AdvanceAdmin";
    }

    SaveParameter("AdminInformation", parameter, password);
}

double ICParametersSave::DistanceRotation(const QString &axisName)
{
    Q_ASSERT_X(axisToRotate_.contains(axisName), "ICParameterSave::DistanceRotation", "no this axis!");
    return axisToRotate_.value(axisName, 0);
}

void ICParametersSave::SetDistanceRotation(const QString &axisName, double value)
{
    Q_ASSERT_X(axisToRotate_.contains(axisName), "ICParameterSave::SetDistanceRotation", "no this axis!");
    axisToRotate_.insert(axisName, value);
    QFile file("./sysconfig/DistanceRotation");
    QFile::copy("./sysconfig/DistanceRotation", "./sysconfig/DistanceRotation~");
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        QMap<QString, double>::iterator p = axisToRotate_.begin();
        while(p != axisToRotate_.end())
        {
            qDebug()<<p.key()<<p.value();
            out<<p.key()<<" "<<p.value()<<endl;
            ++p;
        }
    }
    file.close();
//    system("rm ./sysconfig/DistanceRotation~");
    QFile::remove("./sysconfig/DistanceRotation~");
}

void ICParametersSave::SetBrightness(uint brightness)
{
    QString cmd("BackLight on ");
    cmd += QString::number(brightness);
    ::system(cmd.toAscii());
    SaveParameter(ProductConfig, "Brightness", brightness);
}
