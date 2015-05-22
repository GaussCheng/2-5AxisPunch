#include "icspinbox.h"

#include <QKeyEvent>
#include <QFrame>

QList<int> ICSpinBox::validKeys_(QList<int>()<<Qt::Key_0<<Qt::Key_1<<Qt::Key_2
                                 <<Qt::Key_3<<Qt::Key_4<<Qt::Key_5<<Qt::Key_6
                                 <<Qt::Key_7<<Qt::Key_8<<Qt::Key_9<<Qt::Key_Return
                                 <<Qt::Key_Backspace<<Qt::Key_Left<<Qt::Key_Right
                                 <<Qt::Key_Up<<Qt::Key_Down);
ICSpinBox::ICSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    font_.setPointSize(11);
    SetTextFont(font_);
    setKeyboardTracking(false);
    setButtonSymbols(QSpinBox::NoButtons);
    setAlignment(Qt::AlignRight);
    setKeyboardTracking(false);
    UpdateStatus();
    setFrame(false);
    setMinimumSize(sizeHint());
    setMaximumSize(sizeHint());
}

void ICSpinBox::focusInEvent(QFocusEvent *event)
{
    selectAll();
    oldValue_ = value();
    QSpinBox::focusInEvent(event);
}


void ICSpinBox::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(validKeys_.contains(key))
    {
        QSpinBox::keyPressEvent(event);
        event->accept();
    }
    else
    {
        event->ignore();
        return;
    }
}
