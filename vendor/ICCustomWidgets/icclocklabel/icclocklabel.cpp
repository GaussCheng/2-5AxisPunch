#include "icclocklabel.h"
#include <QLocale>


ICClockLabel::ICClockLabel(QWidget *parent) :
    QLabel(parent)
{
    QFont font(this->font());
    font.setFamily("WenQuanYi");
    font.setPointSize(11);
    font.setPixelSize((font.pointSize() << 2) / 3);
    this->setFont(font);
    ShowTime();
    connect(&timer_,
            SIGNAL(timeout()),
            this,
            SLOT(ShowTime()));
}

void ICClockLabel::showEvent(QShowEvent *e)
{
    timer_.start(1000);
    QLabel::showEvent(e);
}

void ICClockLabel::hideEvent(QHideEvent *e)
{
    timer_.stop();
    QLabel::hideEvent(e);
}

void ICClockLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    emit ClockLableClicked();
    QLabel::mouseReleaseEvent(ev);
}

void ICClockLabel::ShowTime()
{
    dataTime_ = QDateTime::currentDateTime();
    setText(dataTime_.date().toString(Qt::DefaultLocaleLongDate) + "\n" + dataTime_.time().toString());
}
