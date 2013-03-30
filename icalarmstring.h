#ifndef ICALARMSTRING_H
#define ICALARMSTRING_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QFile>

enum LocalAlarm
{
    CommunicationFail   = 200,
    NotInAutoPage       = 201,
    NotInManualPage     = 202
};

/*struct AlarmInfo
{
    AlarmInfo(){};
    AlarmInfo(const QString & name, const QString & reason, const QString & solution)
    {
        alramName = name;
        alarmReason = reason;
        alarmSolution = solution;
    }
    QString alramName;
    QString alarmReason;
    QString alarmSolution;
};*/

class ICAlarmString : public QObject
{
    Q_OBJECT
public:
    static ICAlarmString* Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICAlarmString();
        }
        return instance_;
    }

    inline int PriorAlarmNum(){ return priorAlarmNum_; }
    void SetPriorAlarmNum( int priorAlarmNum );

    inline QMap<int, QString> AlarmInfoMap() { return alarmInfoMap_; }
    inline QString AlarmInfo(int errCode) { return alarmInfoMap_.value(errCode, QString());}
    QString HintInfo(int hintCode) { return hintInfoMap_.value(hintCode, QString());}

    const QString AlarmChineseInfoPathName;
    const QString AlarmEnglishInfoPathName;
    const QString HintChInfoPathName;
    const QString HintEnInfoPathName;

signals:
    void CurrentAlarmNumChanged(int currentAlarmNum);
    void CurrentLanguageChanged();

private slots:
    void OnCurrentLanguageChanged();

private:
    static ICAlarmString * instance_;
    int priorAlarmNum_;
    QMap<int, QString> alarmInfoMap_; //int为报警编号
    QMap<int, QString> hintInfoMap_;
    QFile alarmChInfoFile;
    ICAlarmString();
};

inline void ICAlarmString::SetPriorAlarmNum(int priorAlarmNum)
{
    if(priorAlarmNum_ != priorAlarmNum)
    {
        priorAlarmNum_ = priorAlarmNum;
        if(priorAlarmNum_ != 0)
        {
            emit CurrentAlarmNumChanged(priorAlarmNum_);
        }
    }
}

#endif // ICALARMSTRING_H
