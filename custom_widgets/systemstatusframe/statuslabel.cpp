#include "statuslabel.h"

StatusLabel::StatusLabel(QWidget *parent) :
    QLabel(parent)
{
    this->setStyleSheet("border-radius: 20px;");
}

void StatusLabel::SetStatus(const DisplayStatus & displayStatus)
{
    switch(displayStatus)
    {
    case ONSTATUS:
        {
            if(currentStatus_ != ONSTATUS)
            {
                this->setPixmap(onStatusPixmap_);
                currentStatus_ = ONSTATUS;
            }
        }
        break;
    case OFFSTATUS:
        {
            if(currentStatus_ != OFFSTATUS)
            {
                this->setPixmap(offStatusPixmap_);
                currentStatus_ = OFFSTATUS;
            }
        }
        break;
    case CLEAR:
        {
//            if(currentStatus_ != CLEAR)
//            {
                this->setPixmap(NULL);
                currentStatus_ = CLEAR;
//            }
        }
        break;
    default:
        break;
    }
}

