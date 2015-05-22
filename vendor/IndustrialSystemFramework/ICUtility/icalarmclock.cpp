#include "icalarmclock.h"

ICAlarmClock::ICAlarmClock(QObject* parent) :
    QObject(parent)
{
    Init();
}

void ICAlarmClock::Init()
{    
    QDateTime today = QDateTime::currentDateTime();
    year_ = today.date().year();
    month_ = today.date().month();
    day_ = today.date().day();
    hours_ = 0;
    minutes_ = 0;
    seconds_ = 0;
    timeToAlarm_ = 0;
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(FinishAalarm()));
}

void ICAlarmClock::SetAlarm()
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime alarmTime(QDate(year_, month_, day_), QTime(hours_, minutes_, seconds_));
    timeLeft_ = now.secsTo(alarmTime);
    if (timeLeft_ > 0)
    {
        //qDebug("There are %d seconds to alarm", now.secsTo(alarmTime));
        timeToAlarm_ = timeLeft_*1000;
        StartAlarm();
    }
    else
    {
        //qDebug("There are %d seconds passed", abs(now.secsTo(alarmTime)));
        StopAlarm();
    }
}

void ICAlarmClock::StartAlarm()
{
    if (timer_->isActive())
    {
        timer_->stop();
    }
    else 
    {   
        timer_->start(timeToAlarm_);
    }
}

void ICAlarmClock::StopAlarm()
{
    timer_->stop();
}

void ICAlarmClock::SetYear(int year_)
{
    year_ = year_;
}

void ICAlarmClock::SetMonth(int month_)
{
    month_ = month_;
}

void ICAlarmClock::SetDay(int day_)
{
    day_ = day_;
}

void ICAlarmClock::SetHours(int hours_)
{
    hours_ = hours_;
}

void ICAlarmClock::SetMinutes(int minutes_)
{
    minutes_ = minutes_;
}

void ICAlarmClock::SetSeconds(int seconds_)
{
    seconds_ = seconds_;
}

void ICAlarmClock::FinishAalarm()
{
    timer_->stop();
    emit(ItsTime());
}

int ICAlarmClock::GetTimeLeft()
{
    return timeLeft_;
}

bool ICAlarmClock::IsActive()
{
    return timer_->isActive();
}
