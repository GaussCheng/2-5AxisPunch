#include "ictouchscreentestwidget.h"
#include <QPainter>
#include <QResizeEvent>

ICTouchScreenTestWidget::ICTouchScreenTestWidget(QWidget *parent) :
    QWidget(parent),
    isPressed_(false)
{
}

void ICTouchScreenTestWidget::resizeEvent(QResizeEvent * e)
{
    leftPolygon_.clear();
    leftPolygon_<<QPoint(kLineWidth, 0)<<QPoint(e->size().width(), e->size().height() - kLineWidth)
               <<QPoint(e->size().width() - kLineWidth, e->size().height())<<QPoint(0, kLineWidth);
    rightPolygon_.clear();
    rightPolygon_<<QPoint(e->size().width() - kLineWidth, 0)<<QPoint(e->size().width(), kLineWidth)
                   <<QPoint(kLineWidth, e->size().height())<<QPoint(0, e->size().height() - kLineWidth);
    QWidget::resizeEvent(e);
}

void ICTouchScreenTestWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.setBrush(Qt::green);
    painter.drawPolygon(leftPolygon_);
    painter.drawPolygon(rightPolygon_);
    painter.restore();
    for(int i = 0; i != lines_.size(); ++i)
    {
        painter.drawPolyline(lines_.at(i));
    }
    QWidget::paintEvent(e);
}

void ICTouchScreenTestWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {
        isPressed_ = true;
        lines_.append(QPolygon()<<e->pos());
    }
    QWidget::mousePressEvent(e);
}

void ICTouchScreenTestWidget::mouseReleaseEvent(QMouseEvent *e)
{
    isPressed_ = false;
    QWidget::mouseReleaseEvent(e);
}

void ICTouchScreenTestWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(isPressed_)
    {
        lines_.last().append(e->pos());
        update();
    }
    QWidget::mouseMoveEvent(e);
}
