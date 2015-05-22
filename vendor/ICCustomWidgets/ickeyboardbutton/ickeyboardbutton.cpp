#include "ickeyboardbutton.h"

ICKeyboardButton::ICKeyboardButton(QWidget *parent) :
    QPushButton(parent)
{
    font_.setPointSize(15);
    SetTextFont(font_);
}

ICKeyboardButton::ICKeyboardButton(const QString &text, QWidget *parent) :
    QPushButton(text,parent)
{
    font_.setPointSize(15);
    SetTextFont(font_);
}

ICKeyboardButton::ICKeyboardButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton(icon,text,parent)
{
    font_.setPointSize(15);
    SetTextFont(font_);
}
void ICKeyboardButton::keyPressEvent(QKeyEvent *e)
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
