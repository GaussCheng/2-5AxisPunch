#include "icblinkinglabel.h"

ICBlinkingLabel::ICBlinkingLabel(QWidget *parent) :
    ICLabel(parent),
    textBlinkingTimes_(0),
    backgroundBlinkingTimes_(0)
{
    connect(&textBlinkingTimer_,
            SIGNAL(timeout()),
            SLOT(BlinkColor()));
    connect(&backgroundBlinkingTimer_,
            SIGNAL(timeout()),
            SLOT(BlinkBackGround()));
}

void ICBlinkingLabel::BlinkingColor(QColor color, int times)
{
    textBlinkingTimes_ = times << 1;
    blinkColor_ = color;
    if(textBlinkingTimes_ > 0 && !textBlinkingTimer_.isActive())
    {
        oldStyle_ = this->styleSheet();
        textBlinkingTimer_.start(500);
    }
}

void ICBlinkingLabel::BlinkingBackground(QColor color, int times)
{
    backgroundBlinkingTimes_ = times << 1;
    blinkBackground_ = color;
    if(backgroundBlinkingTimes_ > 0 && !backgroundBlinkingTimer_.isActive())
    {
        oldStyle_ = this->styleSheet();
        backgroundBlinkingTimer_.start(500);
    }

}

void ICBlinkingLabel::BlinkColor()
{
    --textBlinkingTimes_;
    if(textBlinkingTimes_ % 2 == 1)
    {
        this->setStyleSheet(oldStyle_ + QString("color:%1;").arg(blinkColor_.name()));
    }
    else
    {
        this->setStyleSheet(oldStyle_);
    }
    if(textBlinkingTimes_ < 0)
    {
        this->setStyleSheet(oldStyle_);
        textBlinkingTimer_.stop();
    }
}

void ICBlinkingLabel::BlinkBackGround()
{
    --backgroundBlinkingTimes_;
    if(backgroundBlinkingTimes_ %2 == 1)
    {
        this->setStyleSheet(oldStyle_ + QString("background:%1;").arg(blinkBackground_.name()));
    }
    else
    {
        this->setStyleSheet(oldStyle_);
    }
    if(backgroundBlinkingTimes_ < 0)
    {
        this->setStyleSheet(oldStyle_);
        backgroundBlinkingTimer_.stop();
    }
}
