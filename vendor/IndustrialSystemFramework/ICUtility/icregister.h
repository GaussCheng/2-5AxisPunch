#ifndef ICREGISTER_H
#define ICREGISTER_H

#include <QString>
#include <QSettings>
#include <QSharedPointer>
#include <QObject>
#include <QTimer>
#include "ICUtility_global.h"

class ICRegister;
typedef QSharedPointer<ICRegister> ICRegisterPtr;

class ICUTILITYSHARED_EXPORT ICRegister : public QObject
{
    Q_OBJECT
public:
    static ICRegisterPtr Instance()
    {
        if(instance_.isNull())
        {
            instance_ = ICRegisterPtr(new ICRegister());
        }
        return instance_;
    }

    static QString GenerateMachineCode();

    static int Register(const QString& fC, const QString& mC, const QString& code);

    int LeftUseTime() const;

    void SetUseTime(int hour);

    bool IsTryTimeOver() const { return isTimeOver_;}

private slots:
    void OnHourGo();

private:
    QDateTime LastLogTime_() const;
    void SetLastLogTime_(const QDateTime& dt);
    mutable QSettings settings_;
    ICRegister();
    static ICRegisterPtr instance_;

    bool isTimeOver_;
    QTimer checkTimer_;
};

#endif // ICREGISTER_H
