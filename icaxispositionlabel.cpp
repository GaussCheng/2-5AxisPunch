#include <QPainter>
#include <QDebug>

#include "icaxispositionlabel.h"

ICAxisPositionLabel::ICAxisPositionLabel(QWidget *parent) :
    QLabel(parent)
{
    topLeftPoint_.setX(this->rect().left() + 5);
    topLeftPoint_.setY(this->rect().top());
    measureTopLeft_.setX(this->rect().right() + 15);
    measureTopLeft_.setY(this->rect().top());
    posRect_.setRect(topLeftPoint_.x() + 10,
                     topLeftPoint_.y(),
                     this->width() - topLeftPoint_.x(),
                     this->height());
    this->setAttribute(Qt::WA_OpaquePaintEvent);
}

void ICAxisPositionLabel::SetAxisStatusAttribute(const QString &axisName, const QString &measureName)
{
    axisName_ = axisName;
    measureName_ = measureName;
//    update();
//    this->setText(axisName);
}

void ICAxisPositionLabel::SetAxisPosition(const QString & axisPosition)
{
    axisPosition_ = axisPosition;
    update();
}

void ICAxisPositionLabel::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter;
    painter.begin(this);

    painter.drawStaticText(topLeftPoint_, axisName_);
    painter.drawStaticText(measureTopLeft_, measureName_) ;
    painter.drawText(posRect_, Qt::AlignRight, axisPosition_);
    painter.end();
}
