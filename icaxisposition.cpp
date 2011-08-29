#include "icaxisposition.h"

ICAxisPosition::ICAxisPosition(QWidget *parent) :
    QLabel(parent)
{
    this->setText(axisName_);
}
