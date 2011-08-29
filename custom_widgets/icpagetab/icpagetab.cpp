#include <QPainter>
//#include <QMouseEvent>

#include "icpagetab.h"

ICPageTab::ICPageTab(QWidget *parent) :
    QAbstractButton(parent),
    background_(":/resource/pageTabBG.png"),
    checkedBackgroud_(":/resource/pageTabBGChecked.png")
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMaximumSize(sizeHint());
    setMinimumSize(sizeHint());
}

void ICPageTab::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    if(isChecked())
    {
        painter.drawPixmap(this->rect(), checkedBackgroud_);
    }
    else
    {
        painter.drawPixmap(this->rect(), background_);
    }
    if(!icon().isNull())
    {
        painter.drawPixmap((this->width() - iconSize().width()) >> 1,
                           (this->height() - iconSize().height()) >> 1,
                           background_);
    }
    else
    {
        painter.drawText(this->rect(), text(), Qt::AlignHCenter | Qt::AlignVCenter);
    }
    painter.end();

    QWidget::paintEvent(e);
}

//void ICPageTab::mousePressEvent(QMouseEvent *e)
//{
//    if(e->button() == Qt::LeftButton && isCheckable())
//    {
//        setChecked(true);
//        update();
//    }
//    QAbstractButton::mousePressEvent(e);
//}
