#include "icgroupbox.h"
#include <QKeyEvent>

ICGroupBox::ICGroupBox(QWidget *parent) :
    QGroupBox(parent)
{
    QFont f = font();
    f.setPointSize(15);
    SetTextFont(f);
}

void ICGroupBox::keyPressEvent(QKeyEvent *e)
{
    if(isCheckable() && e->key() == Qt::Key_Return)
    {
        setChecked(!isChecked());
    }
    else
    {
        e->ignore();
    }
}
