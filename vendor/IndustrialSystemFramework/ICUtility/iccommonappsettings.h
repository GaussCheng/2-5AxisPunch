#ifndef ICCOMMONAPPSETTINGS_H
#define ICCOMMONAPPSETTINGS_H

#include <QSettings>
#include <QLocale>
#include <QTime>
#ifndef Q_WS_WIN32
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif
#include "ICUtility_global.h"

class ICUTILITYSHARED_EXPORT ICCommonAppSettings : public QSettings
{
public:
    ICCommonAppSettings();
    QLocale Locale();
    void SetLocale(const QLocale &locale);

    uint BacklightTime();
    void SetBacklightTime(uint minutes);

    bool IsLogoutWhenScreenSave();
    void SetLogoutWhenScreenSave(bool isLogout = true);

    uint UsedTime();
    void SetUsedTime(uint hours);

    uint TotalUsedTime();
    void SetTotalUsedTime(uint hours);
    void IncreaseTotalUsedTime();

    QString MachineCode();
    void SetMachineCode(const QString& machineCode);


    QString FactoryCode();
    void SetFactoryCode(const QString& code);

    QString ManufacturingNo();
    void SetManufacturingNo(const QString& code);

    QString ManufacturingDate();
    void SetManufacturingDate(const QString& code);

    bool KeyTone();// { return GetParameter(ProductConfig, "KeyTone", true).toBool();}
#ifndef Q_WS_WIN32
    void SetKeyTone(bool isOn);//  {SaveParameter(ProductConfig, "KeyTone", isOn);ioctl(beepFD_, 0, isOn ? 20 : 10);}
#else
    void SetKeyTone(bool isOn);//  {SaveParameter(ProductConfig, "KeyTone", isOn);}
#endif


    static QTime StartupTime(){ return startupTime_;}
    static void SetStartupTime(const QTime& time) { startupTime_ = time;}
//    static uint Used
    const static char* UsbPath;
private:
    const static char* SystemConfigGroup;
    const static char* SessionGroup;
    const static char* LocaleGroup;

    static QTime startupTime_;
};

inline QLocale ICCommonAppSettings::Locale()
{
    beginGroup(LocaleGroup);
    QLocale ret =  value("Locale", QLocale(QLocale::Chinese, QLocale::China)).toLocale();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetLocale(const QLocale &locale)
{
    beginGroup(LocaleGroup);
    setValue("Locale", locale);
    endGroup();
}

inline uint ICCommonAppSettings::BacklightTime()
{
    beginGroup(SystemConfigGroup);
    uint ret = value("BacklightTime", 5).toUInt();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetBacklightTime(uint minutes)
{
    beginGroup(SystemConfigGroup);
    setValue("BacklightTime", minutes);
    endGroup();
}

inline bool ICCommonAppSettings::IsLogoutWhenScreenSave()
{
    beginGroup(SystemConfigGroup);
    bool ret = value("IsLogoutWhenScreenSave", true).toBool();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetLogoutWhenScreenSave(bool isLogout)
{
    beginGroup(SystemConfigGroup);
    setValue("IsLogoutWhenScreenSave", isLogout);
    endGroup();
}

inline uint ICCommonAppSettings::UsedTime()
{
    beginGroup(SystemConfigGroup);
    uint ret = value("UsedTime", 0).toUInt();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetUsedTime(uint hours)
{
    beginGroup(SystemConfigGroup);
    setValue("UsedTime", hours);
    endGroup();
}

inline uint ICCommonAppSettings::TotalUsedTime()
{
    beginGroup(SessionGroup);
    uint ret = value("TotalUsedTime", 0).toUInt();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetTotalUsedTime(uint hours)
{
    beginGroup(SessionGroup);
    setValue("TotalUsedTime", hours);
    endGroup();
}


inline void ICCommonAppSettings::IncreaseTotalUsedTime()
{
    SetTotalUsedTime(TotalUsedTime() + 1);
}

inline QString ICCommonAppSettings::MachineCode()
{
    beginGroup(SessionGroup);
    QString ret = value("MachineCode", "").toString();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetMachineCode(const QString &machineCode)
{
    beginGroup(SessionGroup);
    setValue("MachineCode", machineCode);
    endGroup();
}


inline QString ICCommonAppSettings::FactoryCode()
{
    beginGroup(SessionGroup);
    QString ret = value("FactoryCode", "123456").toString();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetFactoryCode(const QString &code)
{
    beginGroup(SessionGroup);
    setValue("FactoryCode", code);
    endGroup();
}

inline QString ICCommonAppSettings::ManufacturingNo()
{
    beginGroup(SessionGroup);
    QString ret = value("ManufacturingNo", "").toString();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetManufacturingNo(const QString &code)
{
    beginGroup(SessionGroup);
    setValue("ManufacturingNo", code);
    endGroup();
}

inline QString ICCommonAppSettings::ManufacturingDate()
{
    beginGroup(SessionGroup);
    QString ret = value("ManufacturingDate", "").toString();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetManufacturingDate(const QString &code)
{
    beginGroup(SessionGroup);
    setValue("ManufacturingDate", code);
    endGroup();
}

inline bool ICCommonAppSettings::KeyTone()
{
    beginGroup(SessionGroup);
    bool ret = value("KeyTone", true).toBool();
    endGroup();
    return ret;
}

inline void ICCommonAppSettings::SetKeyTone(bool isOn)
{
    beginGroup(SessionGroup);
    setValue("KeyTone", isOn);
    endGroup();
#ifndef Q_WS_WIN32
    int beepFD = open("/dev/szhc_beep", O_WRONLY);
    ioctl(beepFD, 0, isOn ? 20 : 10);
    close(beepFD);
#endif

}

#endif // ICCOMMONAPPSETTINGS_H
