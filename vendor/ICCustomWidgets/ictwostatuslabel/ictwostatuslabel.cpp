#include "ictwostatuslabel.h"

#include <QTimer>

ICTwoStatusLabel::ICTwoStatusLabel(QWidget *parent) :
    QLabel(parent),
    timer_(NULL),
    count_(0),
    endCount_(0)
{
}

ICTwoStatusLabel::~ICTwoStatusLabel()
{
    if(timer_ != NULL)
    {
        delete timer_;
    }
}

void ICTwoStatusLabel::Start(int count)
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
    }
}

void ICTwoStatusLabel::Stop()
{
    if(timer_ != NULL)
    {
        timer_->stop();
    }
}

void ICTwoStatusLabel::OnTimerOut()
{
    if(endCount_ == count_)
    {
        timer_->stop();
        ShowOffStatus();
        return;
    }
    if(count_ % 2 == 0)
    {
        ShowOnStatus();
    }
    else
    {
        ShowOffStatus();
    }
    ++count_;
}
