#ifndef ICPARAMETERSSAVE_H
#define ICPARAMETERSSAVE_H

#include <QSettings>
#include <QString>
#include <QLocale>
#include <QTranslator>
#include <fcntl.h>
#include <sys/ioctl.h>

class ICParametersSave : public QSettings
{
    Q_OBJECT
public:

    enum OperationLevel
    {
        MachineOperator,
        MachineAdmin,
        AdvanceAdmin
    };
    static ICParametersSave * Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICParametersSave("sysconfig/systemParameter.hc");
        }
        return instance_;
    }
    ~ICParametersSave();

    void SaveParameter(const QString & group, const QString & key, const QVariant & value);
    QVariant GetParameter(const QString & group, const QString & key, const QVariant & defaultValue);

    void SetCommunicationConfig(const QString& device,
                                int baudRate,
                                const QString& parity,
                                int dataBit,
                                int stopBit);
    QString Device(){ return GetParameter(CommunicationConfig, "Device", "/dev/ttyS0").toString();}
    int BaudRate(){ return GetParameter(CommunicationConfig, "BaudRate", 57600).toInt();}
    QString Parity() { return GetParameter(CommunicationConfig, "Parity", "none").toString();}
    int DataBit() { return GetParameter(CommunicationConfig, "DataBit", 8).toInt();}
    int StopBit(){ return GetParameter(CommunicationConfig, "StopBit", 1).toInt();}

    QString MoldName(const QString & defaultMoldName) { return GetParameter("BootParameter", "moldName", defaultMoldName).toString(); }
    void SetMoldName(const QString & moldName) { SaveParameter("BootParameter", "moldName", moldName); }

    QLocale::Language Language(){ return static_cast<QLocale::Language>(GetParameter(SystemLocale, "SystemLanguage", QLocale::Chinese).toInt());}
    void SetLanguage(QLocale::Language language);
    void LoadInitLocale() { SetCountry(Country());}
    QLocale::Country Country() { return static_cast<QLocale::Country>(GetParameter(SystemLocale, "SystemCountry", QLocale::China).toInt());}
    void SetCountry(QLocale::Country country);

    QTranslator* Translator() { return translator_;}

    bool KeyTone() { return GetParameter(ProductConfig, "KeyTone", true).toBool();}
    void SetKeyTone(bool isOn)  {SaveParameter(ProductConfig, "KeyTone", isOn);ioctl(beepFD_, 0, isOn ? 20 : 10);}

    bool VerifyPassword(OperationLevel level, const QString& password);
    void SetPassword(OperationLevel level, const QString& password);

    double DistanceRotation(const QString& axisName);/* {return GetParameter("AxisConfig", axisName, 0).toDouble();}*/
    void SetDistanceRotation(const QString& axisName, double value);/* { SaveParameter("AxisConfig", axisName, value);}*/
    //    void SetFileName(const QString & fileName);
    bool IsExtentFunctionUsed() {return GetParameter(SystemMachine, "AdminSettings", false).toBool();}
    void SetExtentFunctionUsed(bool isUsed) {SaveParameter(SystemMachine, "AdminSettings", isUsed);}

    uint BackLightTime() { return GetParameter(ProductConfig, "BackLight", 5).toUInt();}
    void SetBackLightTime(uint time) { SaveParameter(ProductConfig, "BackLight", time);}

    uint Brightness() { return GetParameter(ProductConfig, "Brightness", 0).toUInt();}
    void SetBrightness(uint brightness);

    bool IsSingleArm() { return GetParameter(SystemMachine, "ArmNum", false).toBool();}
    void SetSingleArm(bool isSingle) { SaveParameter(SystemMachine, "ArmNum", isSingle);}

signals:
    void CurrentLanguageChanged();

public:
    const QString ProductOperationer;
    const QString ProductAdministrator;
    const QString ProductAlarmHistory;
    const QString SystemMachine;
    const QString SystemLocale;
    const QString InstructMultidotPut;
    const QString CommunicationConfig;
    const QString ProductConfig;

private:
    static ICParametersSave* instance_;
    QTranslator *translator_;
    QMap<QString, double> axisToRotate_;
    int beepFD_;
    ICParametersSave(const QString fileName = QString());
};



#endif // ICPARAMETERSSAVE_H
