#include "icunitlabel.h"
#include <QPainter>

ICUnitLabel::ICUnitLabel(QWidget *parent) :
    QLabel(parent),
    oldValue_(-1)
{
    QFont f = font();
    f.setPointSize(15);
    SetTextFont(f);
    this->setTextFormat(Qt::PlainText);
    setAlignment(Qt::AlignRight);
}

//void ICUnitLabel::paintEvent(QPaintEvent *e)
//{
//    Q_UNUSED(e)
//    QPainter painter;
//    painter.begin(this);
//    painter.drawText(this->rect(), text() + " "+ Suffix(), QTextOption(Qt::AlignLeft));
//    painter.end();
//}
