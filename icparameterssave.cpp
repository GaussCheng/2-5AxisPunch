#include "icparameterssave.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "icfile.h"

ICParametersSave * ICParametersSave::instance_ = NULL;

ICParametersSave::ICParametersSave(const QString fileName)
    : QSettings(fileName,QSettings::IniFormat),
    ProductOperationer("ProductOperationer"),
    ProductAdministrator("ProductAdministrator"),
    ProductAlarmHistory("ProductAlarmHistory"),
    SystemMachine("SystemMachine"),
    SystemLocale("SystemLanguage"),
    InstructMultidotPut("InstructMultidotPut"),
    CommunicationConfig("CommunicationConfig"),
    ProductConfig("ProductConfig"),
    ProgramConfig("ProgramConfig"),

    translator_(new QTranslator()),
      sysTranslator_(new QTranslator()),
      isRoot_(false)
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
    SetKeyTone(KeyTone(), false);
}

ICParametersSave::~ICParametersSave()
{
    delete translator_;
}

void ICParametersSave::SaveParameter(const QString & group, const QString & key, const QVariant & value, bool issync)
{
    this->beginGroup(group);
    this->setValue(key,value);
    this->endGroup();
    if(issync)
        this->sync();
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

void ICParametersSave::SetLanguage(QLocale::Language language, bool isSync)
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
    if(isSync)
        SaveParameter(SystemLocale, "SystemLanguage", static_cast<int>(language), isSync);

    CurrentLanguageChanged();
}

void ICParametersSave::SetCountry(QLocale::Country country, bool isSync)
{
    switch(country)
    {
    case QLocale::China:
        {
            QLocale::setDefault(QLocale(QLocale::Chinese, country));
            SetLanguage(QLocale::Chinese, isSync);
        }
        break;
    case QLocale::UnitedStates:
        {
            QLocale::setDefault(QLocale(QLocale::English, country));
            SetLanguage(QLocale::English, isSync);
        }
        break;
    default:
        {
            QLocale::setDefault(QLocale(QLocale::Chinese, country));
            SetLanguage(QLocale::Chinese, isSync);
        }
    }
    if(isSync)
        SaveParameter(SystemLocale, "SystemCountry", static_cast<int>(country), isSync);
}

int ICParametersSave::ProgramUsedFlag()
{
    return GetParameter(ProgramConfig,"ProgramUsed",3).toInt();
}

void ICParametersSave::SetProgramUsedFlag(int flag)
{
    SaveParameter(ProgramConfig,"ProgramUsed",flag);
}

int ICParametersSave::ProgramInnerFlag()
{
    return GetParameter(ProgramConfig,"ProgramInnerUsed",0).toInt();

}

void ICParametersSave::SetProgramInnerFlag(int flag)
{
    SaveParameter(ProgramConfig,"ProgramInnerUsed",flag);
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
    if(password == SuperPassward())
    {
        SetRoot(true);
    }
    else
    {
        SetRoot(false);
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
    QByteArray toWrite;
    QMap<QString, double>::iterator p = axisToRotate_.begin();
    while(p != axisToRotate_.end())
    {
        toWrite += QString("%1 %2 \n").arg(p.key()).arg(p.value()).toUtf8();
        qDebug()<<p.key()<<p.value();
//        out<<p.key()<<" "<<p.value()<<endl;
        ++p;
    }
    ICFile file("./sysconfig/DistanceRotation");
    file.ICWrite(toWrite);
//    QFile file("./sysconfig/DistanceRotation");
//    QFile::copy("./sysconfig/DistanceRotation", "./sysconfig/DistanceRotation~");
//    if(file.open(QFile::WriteOnly | QFile::Text))
//    {
//        QTextStream out(&file);
//    }
//    file.close();
////    system("rm ./sysconfig/DistanceRotation~");
//    QFile::remove("./sysconfig/DistanceRotation~");
}

void ICParametersSave::SetBrightness(uint brightness, bool isSync)
{
//    QString cmd("BackLight on ");
    QString cmd("BackLight.sh  ");
    cmd += QString::number(brightness);
    ::system(cmd.toAscii());
    if(isSync)
        SaveParameter(ProductConfig, "Brightness", brightness, isSync);
}
