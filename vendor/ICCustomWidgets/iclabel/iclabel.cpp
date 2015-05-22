#include "iclabel.h"
#include <QPainter>

ICLabel::ICLabel(QWidget *parent) :
    QLabel(parent),
    enSpecialText_(false)
{
//    QFont f = font();
//    f.setPointSize(15);
//    SetTextFont(f);
    this->setTextFormat(Qt::PlainText);
//    this->setAttribute(Qt::WA_OpaquePaintEvent);
//    this->setAttribute(Qt::WA_StaticContents);
}


