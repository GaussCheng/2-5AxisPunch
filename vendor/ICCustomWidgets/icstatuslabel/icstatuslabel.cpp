#include "icstatuslabel.h"

#include <QPainter>
#include <QDebug>

ICStatusLabel::ICStatusLabel(QWidget *parent) :
    QFrame(parent),
    enSpecialText_(false)
{
    unitFont_.setPointSize(9);
    SetUnitFont(unitFont_);
    valueFont_.setPointSize(13);
    SetValueFont(valueFont_);
}

void ICStatusLabel::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawPixmap(0, 0, IconSize().width(), IconSize().height(), Icon());
    painter.save();
    painter.setFont(UnitFont());
    painter.setPen(Qt::white);
    const int textStartLeft = IconSize().width() + 2;
    painter.drawStaticText(textStartLeft, 0, UnitText());
    painter.setFont(ValueFont());
    painter.setPen(Qt::yellow);
//    if(!EnSpecialText())
//    {
    painter.drawText(textStartLeft, height() - 4, Text());
//    }
//    else
//    {
//        painter.drawText(textStartLeft, height() - 4, SpecialText());
//    }
    painter.restore();
}
