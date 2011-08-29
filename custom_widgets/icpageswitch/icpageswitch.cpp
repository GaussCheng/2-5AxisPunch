#include <QPainter>
#include <QBitmap>
#include "icpageswitch.h"

ICPageSwitch::ICPageSwitch(QWidget *parent) :
    QAbstractButton(parent),
    background_(":/resource/menuBtnBG.png"),
    disableBG_(":/resource/menuBtnDisableBG.png"),
    checkedBG_(":/resource/menuBtnCheckedBG.png")
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setIconSize(QSize(48, 48));
    setMaximumSize(sizeHint());
    setMinimumSize(sizeHint());
}

ICPageSwitch::~ICPageSwitch()
{
}

void ICPageSwitch::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    if(isEnabled())
    {
        if(isChecked())
        {
            painter.drawPixmap(this->rect(), disableBG_);
        }
        else
        {
            painter.drawPixmap(this->rect(), background_);
        }
        painter.drawPixmap(((this->width() - iconSize().width()) >> 1), 15, icon().pixmap(iconSize()));
    }
    else
    {
        painter.drawPixmap(this->rect(), disableBG_);
        painter.drawPixmap(((this->width() - iconSize().width()) >> 1), 15, icon().pixmap(iconSize(), QIcon::Disabled));
    }
    int lastHeight = iconSize().height() + 15;
    painter.drawText(QRect(0, lastHeight, this->width(), this->height() - lastHeight), text() , Qt::AlignHCenter | Qt::AlignVCenter);
    painter.end();
    QWidget::paintEvent(e);
}
