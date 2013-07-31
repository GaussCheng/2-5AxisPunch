#include "speedstatuslabel.h"
#include <QTimer>
#include "operatingratiosetdialog.h"

SpeedStatusLabel::SpeedStatusLabel(QWidget *parent) :
    QLabel(parent),
    timer_(NULL)
{
//    this->setStyleSheet("border-radius: 20px;");
}

void SpeedStatusLabel::mousePressEvent(QMouseEvent *ev)
{
    if(count_ % 2 == 0)
    {
        oldSytle_ = this->styleSheet();
    }
    this->setStyleSheet("background-color:blue;color:white;");
}

void SpeedStatusLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    this->setStyleSheet(oldSytle_);
    OperatingRatioSetDialog::Instance()->show();
}

void SpeedStatusLabel::SetCurrentSpeed(const QString & speed)
{
    this->setText(speed);
    Start(5);
}

void SpeedStatusLabel::Start(int count)
{
    if(timer_ == NULL)
    {
        timer_ = new QTimer();
        connect(timer_,
                SIGNAL(timeout()),
                this,
                SLOT(OnTimerOut()));
    }
    if(!timer_->isActive())
    {
        endCount_ = count;
        count_ = 0;
        timer_->start(500);
        oldSytle_ = this->styleSheet();
    }
}

void SpeedStatusLabel::Stop()
{
    if(timer_ != NULL)
    {
        timer_->stop();
    }
}

void SpeedStatusLabel::OnTimerOut()
{
    if(endCount_ == count_)
    {
        timer_->stop();
        this->setStyleSheet(oldSytle_);
        return;
    }
    if(count_ % 2 == 0)
    {
//        ShowOnStatus();
        this->setStyleSheet("color: rgb(0, 0, 127);background-color: rgb(85, 255, 127);");
    }
    else
    {
//        ShowOffStatus();
        this->setStyleSheet(oldSytle_);
    }
    ++count_;
}
