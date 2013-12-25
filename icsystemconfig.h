#ifndef ICSYSTEMCONFIG_H
#define ICSYSTEMCONFIG_H

#include <QObject>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QMap>
#include <QStringList>

class QSettings;

class ICSerialPortConfig
{
public:
    QString DeviceName() const { return deviceName_;}
    void SetDevice(const QString &device) { deviceName_ = device;}
    int BaudRate() const { return baudRate_;}
    void SetBaudRate(int baudRate) { baudRate_ = baudRate;}
    QString Parity() const { return parity_;}
    void SetParity(const QString & parity) { parity_ = parity;}
    int DataBit() const { return dataBit_;}
    void SetDataBit(int dataBit) { dataBit_ = dataBit;}
    int StopBit() const { return stopBit_;}
    void SetStopBit(int stopBit) { stopBit_ = stopBit;}
private:
    QString deviceName_;
    int baudRate_;
    QString parity_;
    int dataBit_;
    int stopBit_;
};

class ICSystemConfig : public QObject
{
    Q_OBJECT
public:
    explicit ICSystemConfig(QObject *parent = 0);

    ICSerialPortConfig SerialPortConfig() const;
    void SetSerialPortConfig(ICSerialPortConfig config);

    QString MoldName() const;
    void SetMoldName(const QString &name);

signals:

public slots:

private:
    const QString CommunicationConfigGroup;
    const QString MoldNameConfigGroup;
    mutable QSettings *sysSetting_;

};

typedef QMap<QString, QString> LocaleStringMap;
static inline QString GetLocalNameFromMap(const LocaleStringMap& map, const QString& languageName) { return (map.value(languageName, ""));}
struct ICUserIOInfo
{
    int hardwarePoint;
    QString code;
    LocaleStringMap localeNameMap;
    QString GetLocaleName(const QString& languageName) const { return GetLocalNameFromMap(localeNameMap, languageName);}
};

struct ICUserPointInfo
{
    LocaleStringMap localeNameMap;
    QString GetLocaleName(const QString& languageName) const { return GetLocalNameFromMap(localeNameMap, languageName);}
};

struct ICUserActionInfo
{
    int type;
    int pointNum;
    LocaleStringMap localeNameMap;
    bool dir;
    QString GetLocaleName(const QString& languageName) const { return GetLocalNameFromMap(localeNameMap, languageName);}
};

class ICUserDefineConfig
{
public:
    typedef QSharedPointer<ICUserDefineConfig> ICUserDefineConfigSPTR;
    typedef QWeakPointer<ICUserDefineConfig> ICUserDefineConfigWPTR;
    typedef QMap<int, ICUserIOInfo> UserIOInfos;
    typedef QMap<int, ICUserPointInfo> UserPointInfos;
    typedef QMap<int, ICUserActionInfo> UserActionInfos;

    static ICUserDefineConfigSPTR Instance()
    {
        if(instance_.isNull())
        {
            instance_ = ICUserDefineConfigSPTR(new ICUserDefineConfig());
        }
        return instance_;
    }

    void Init();
    QString GetPointsLocaleName(int id, const QString& languageName = "zh")
    {
        if(!pointInfos_.contains(id)) return "";
        return pointInfos_.value(id).GetLocaleName(languageName);
    }

    QString GetIOActionLocaleNameByID(int id, const QString& languageName = "zh")
    {
        if(!actionInfos_.contains(id)) return "";
        return actionInfos_.value(id).GetLocaleName(languageName);
    }

    QString GetIOActionShortcutLocaleNameByID(int id, const QString& languageName = "zh")
    {
        if(!actionShortcut_.contains(id)) return "";
        return actionShortcut_.value(id).GetLocaleName(languageName);
    }

    QString GetIOActionLocaleName(int type, int id , bool dir, const QString& languageName = "zh");

    int GetIOActionType(int id)
    {
        if(!actionInfos_.contains(id)) return 0;
        return actionInfos_.value(id).type;
    }

    ICUserActionInfo GetActionByID(int id){return actionInfos_.value(id, ICUserActionInfo());}
    ICUserActionInfo GetActionShortcutByID(int id) { return actionShortcut_.value(id, ICUserActionInfo());}

    QList<ICUserActionInfo> GetActionInfosByType(int type = -1);

    QList<ICUserIOInfo> AllXInfos() { return xInfos_.values();}
    QList<ICUserIOInfo> AllYInfos() { return yInfos_.values();}
    QList<ICUserIOInfo> AllEuXInfos() { return this->euxInfos_.values();}
    QList<ICUserIOInfo> AllEuYInfos() { return this->euyInfos_.values();}
    QStringList AllEuXStrings() { return this->euxStrings_;}
    QStringList AllEuYStrings() { return this->euyStrings_;}
    ICUserIOInfo XInfo(int id) { return xInfos_.value(id, ICUserIOInfo());}
    ICUserIOInfo YInfo(int id) { return yInfos_.value(id, ICUserIOInfo());}
    ICUserIOInfo EuXinfo(int id) { return euxInfos_.value(id, ICUserIOInfo());}
    ICUserIOInfo EuYinfo(int id) { return euyInfos_.value(id, ICUserIOInfo());}
    QString EuXString(int id) { return euxStrings_.at(id);}
    QString EuYString(int id) { return euyStrings_.at(id);}

private:
    explicit ICUserDefineConfig(){}
    static ICUserDefineConfigSPTR instance_;

private:
    void ReadIOInfos_(const QString& path, UserIOInfos& infos);
    void ReadPointInfos_(const QString& path);
    void ReadActionInfos_(const QString& path, UserActionInfos& infos);
    UserIOInfos xInfos_;
    UserIOInfos yInfos_;
    UserIOInfos euxInfos_;
    UserIOInfos euyInfos_;
    UserPointInfos pointInfos_;
    UserActionInfos actionInfos_;
    UserActionInfos actionShortcut_;
    QStringList euxStrings_;
    QStringList euyStrings_;
};

typedef ICUserDefineConfig::ICUserDefineConfigSPTR ICUserDefineConfigSPTR;
typedef ICUserDefineConfig::ICUserDefineConfigWPTR ICUserDefineConfigWPTR;

#endif // ICSYSTEMCONFIG_H
