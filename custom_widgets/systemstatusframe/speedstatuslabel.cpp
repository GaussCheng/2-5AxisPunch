#include "speedstatuslabel.h"

#include "operatingratiosetdialog.h"

SpeedStatusLabel::SpeedStatusLabel(QWidget *parent) :
    QLabel(parent)
{
    this->setStyleSheet("border-radius: 20px;");
}

void SpeedStatusLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    OperatingRatioSetDialog::Instance()->show();
}

