#include "iccheckbox.h"

#include <QKeyEvent>
#include <iostream>

ICCheckBox::ICCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    QFont f = font();
    f.setPointSize(15);
    SetTextFont(f);
//    setStyleSheet(style_);
    setFocusPolicy(Qt::NoFocus);
//    setAttribute(Qt::WA_OpaquePaintEvent);
}

ICCheckBox::ICCheckBox(const QString &text, QWidget *parent):
    QCheckBox(text, parent)
{
//    QFont f = font();
//    f.setPointSize(15);
//    SetTextFont(f);
//    setStyleSheet(style_);
    setFocusPolicy(Qt::NoFocus);
//    setAttribute(Qt::WA_OpaquePaintEvent);
}

void ICCheckBox::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return)
    {
        if(!isTristate())
        {
            toggle();
        }
        else
        {
            nextCheckState();
        }
    }
    else
    {
        e->ignore();
    }
}

void ICCheckBox::focusOutEvent(QFocusEvent *e)
{
    emit FocusOut();
    QCheckBox::focusOutEvent(e);
}
