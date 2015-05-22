#ifndef ICALARMCLOCK_H
#define ICALARMCLOCK_H

#include <QObject>
#include <QTimer> 
#include <QDateTime>
#include <QCoreApplication>
#include "ICUtility_global.h"

class ICUTILITYSHARED_EXPORT ICAlarmClock : public QObject
{  
  Q_OBJECT     
    public:
        ICAlarmClock(QObject* = 0);
        void StartAlarm();
        void StopAlarm();
        void SetYear(int);
        void SetMonth(int);
        void SetDay(int);
        void SetHours(int);
        void SetMinutes(int);
        void SetSeconds(int);
        void SetAlarm();
        void Init();
        int GetTimeLeft();
        bool IsActive();
    
    private:    
        int year_;
        int month_;
        int day_;
        int hours_;
        int minutes_;
        int seconds_;
        int timeToAlarm_;
        int timeLeft_;
        QTimer *timer_;
        
    private slots:
        void FinishAalarm();
        
    signals:
        void ItsTime();
    
};

#endif
