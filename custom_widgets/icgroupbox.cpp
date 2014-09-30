#include "icgroupbox.h"
#include <QKeyEvent>

ICGroupBox::ICGroupBox(QWidget *parent) :
    QGroupBox(parent)
{
}


void ICGroupBox::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void ICGroupBox::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
}
