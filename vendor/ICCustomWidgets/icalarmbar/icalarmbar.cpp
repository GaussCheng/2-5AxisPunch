#include "icalarmbar.h"
#include "icoptimize.h"

ICAlarmBar::ICAlarmBar(QWidget *parent) :
    QLabel(parent),
    currentAlarmIndex_(0)
{
    font_.setPointSize(12);
    SetTextFont(font_);
    connect(&timer_,
            SIGNAL(timeout()),
            SLOT(ShowNextAlarm()));
}

void ICAlarmBar::showEvent(QShowEvent *e)
{
    timer_.start(1000);
    QLabel::showEvent(e);
}

void ICAlarmBar::hideEvent(QHideEvent *e)
{
    timer_.stop();
    QLabel::hideEvent(e);
}

void ICAlarmBar::ShowNextAlarm()
{
    if(likely(alarmStrings_.isEmpty()))
    {
        clear();
        return;
    }
    setText(alarmStrings_.at(currentAlarmIndex_));
    currentAlarmIndex_ = (currentAlarmIndex_ + 1) % alarmStrings_.size();
}
