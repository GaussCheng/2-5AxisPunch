#include "icbutton.h"
#include <QKeyEvent>

ICButton::ICButton(QWidget *parent) :
    QPushButton(parent),
    _instruction(""),
    _paramter("")
{
    QFont f = font();
    f.setPointSize(12);
    SetTextFont(f);
    setFocusPolicy(Qt::NoFocus);
}

void ICButton::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return)
    {
        this->click();
        e->accept();
    }
    else
    {
        QPushButton::keyPressEvent(e);
    }
}
