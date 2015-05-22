#include "icbarrel.h"

#include <QPainter>

ICBarrel::ICBarrel(QWidget *parent) :
    QFrame(parent),
    barrelValues_(DefaultBarrelCount)
{
    SetBarrelCount(2);
    QFont myfont = this->font();
    myfont.setPointSize(12);
    myfont.setPixelSize((myfont.pointSize() << 2) / 3);
    this->setFont(myfont);
    setMinimumHeight(head_.height());
}

void ICBarrel::paintEvent(QPaintEvent *)
{
    if(BarrelCount() == 0)
    {
        return;
    }
    QPainter painter(this);
    painter.drawPixmap(0, 0, head_);
    const int picHeight = head_.height() - 5;
    painter.save();
    painter.setPen(colors_[0]);
//    painter.drawRect(0, 0, head_.width(), picHeight);
    if(BarrelValue(0) != specialValues_[0])
    {
        painter.drawText(2, 0, head_.width(), picHeight,  Qt::AlignCenter, BarrelText(0));
    }
    else
    {
        painter.drawText(2, 0, head_.width(), picHeight,  Qt::AlignCenter, specialValueTexts_[0]);
    }
    const int bc = BarrelCount();
    int startWidth;
    if(bc > 1)
    {
        if (bc > 2)
        {
            for(int i = 0; i < bc - 2; ++i)
            {
                startWidth = head_.width() + (i * mid_.width());
                painter.drawPixmap(startWidth, 0, mid_);
                painter.setPen(colors_[i + 1]);
                if(BarrelValue(i + 1) != specialValues_[i + 1])
                {
                    painter.drawText(startWidth, 0, mid_.width(), picHeight, Qt::AlignCenter, BarrelText(i + 1));
                }
                else
                {
                    painter.drawText(startWidth, 0, mid_.width(), picHeight, Qt::AlignCenter, specialValueTexts_[i + 1]);
                }
            }
        }
        startWidth = head_.width() + (bc - 2) * mid_.width();
        painter.drawPixmap(startWidth, 0, tail_);
        painter.setPen(colors_[BarrelCount() - 1]);
        if(BarrelValue(BarrelCount() - 1) != specialValues_[BarrelCount() - 1])
        {
            painter.drawText(startWidth, 0, tail_.width(), picHeight, Qt::AlignCenter, BarrelText(BarrelCount() - 1));
        }
        else
        {
            painter.drawText(startWidth, 0, tail_.width(), picHeight, Qt::AlignCenter, specialValueTexts_[BarrelCount() - 1]);
        }
    }
    painter.restore();
//    QFrame::paintEvent(e);
}

void ICBarrel::SetBarrelCount(int count)
{
    if(count < 0)
    {
        count = 0;
    }
    barrelCount_ = count;
    if(specialValues_.size() < count)
    {
        barrelValues_.resize(count);
        specialValues_.resize(count);
        specialValueTexts_.resize(count);
        colors_.resize(count);
    }
    for(int i = 0; i != count; ++i)
    {
        barrelValues_[i] = 0;
//        specialValues_[i] = -1;
    }
    if(count < 2)
    {
        setMinimumWidth(head_.width());
    }
    else
    {
        setMinimumWidth(head_.width() + tail_.width() + (count - 2) * mid_.width());
    }
}
