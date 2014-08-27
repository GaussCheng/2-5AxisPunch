#include "iccombobox.h"

#include <QKeyEvent>

ICComboBox::ICComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void ICComboBox::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void ICComboBox::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
}
