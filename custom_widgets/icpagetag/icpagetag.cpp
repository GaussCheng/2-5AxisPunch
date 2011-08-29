#include <QPainter>
#include <QLabel>

#include "icpagetag.h"

ICPageTag::ICPageTag(QWidget *parent) :
    QAbstractButton(parent),
    leftBackground_(":/resource/leftTagBG.png"),
    rightBackground_(":/resource/rightTagBG.png"),
    leftPressBG_(":/resource/leftTagBG_Pressed.png"),
    rightPressBG_(":/resource/rightTagBG_Pressed.png")
{
    iconSide_ = IconOnLeft;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMaximumSize(sizeHint());
    setMinimumSize(sizeHint());
}

ICPageTag::~ICPageTag()
{
}

void ICPageTag::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    if(Direction() == IconOnLeft)
    {
        painter.begin(this);
        if(!isDown())
        {
            painter.drawPixmap(this->rect(), leftBackground_);
        }
        else
        {
            painter.drawPixmap(this->rect(), leftPressBG_);
        }
        painter.drawPixmap(90, 10, icon().pixmap(iconSize()));
        painter.drawText(QRect(0, 10, this->width() - 150, iconSize().height()), text(), Qt::AlignRight | Qt::AlignVCenter);
        painter.end();
    }
    else
    {
        painter.begin(this);
        if(!isDown())
        {
            painter.drawPixmap(this->rect(), rightBackground_);
        }
        else
        {
            painter.drawPixmap(this->rect(), rightPressBG_);
        }
        painter.drawText(QRect(110, 10, this->width(), iconSize().height()), text(), Qt::AlignLeft | Qt::AlignVCenter);
        painter.drawPixmap(this->rect().right() - iconSize().width() - 130, 10, icon().pixmap(iconSize()));
        painter.end();
    }
    QWidget::paintEvent(e);
}
