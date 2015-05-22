#include "icdoublespinbox.h"

#include <QFocusEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QDebug>

QList<int> ICDoubleSpinBox::validKeys_(QList<int>()<<Qt::Key_0<<Qt::Key_1<<Qt::Key_2
                                 <<Qt::Key_3<<Qt::Key_4<<Qt::Key_5<<Qt::Key_6
                                 <<Qt::Key_7<<Qt::Key_8<<Qt::Key_9<<Qt::Key_Return
                                 <<Qt::Key_Backspace<<Qt::Key_Left<<Qt::Key_Right
                                 <<Qt::Key_Up<<Qt::Key_Down<<Qt::Key_Period);
ICDoubleSpinBox::ICDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
    font_.setPointSize(11);
    SetTextFont(font_);
    setKeyboardTracking(false);
    setButtonSymbols(QDoubleSpinBox::NoButtons);
    setAlignment(Qt::AlignRight);
    setKeyboardTracking(false);
    UpdateStatus();
    setFrame(false);
    setMinimumSize(sizeHint());
    setMaximumSize(sizeHint());
//    connect(this,
//            SIGNAL(editingFinished()),
//            this,
//            SLOT(OnFinished()));
//    connect(lineEdit(),
//            SIGNAL(textChanged(QString)),
//            this,
//            SLOT(OnLineEditTextChanged(QString)));
}

void ICDoubleSpinBox::focusInEvent(QFocusEvent *event)
{
    selectAll();
    oldValue_ = value();
    QDoubleSpinBox::focusInEvent(event);
}

void ICDoubleSpinBox::keyPressEvent(QKeyEvent *event)
{

    int key = event->key();
    if(validKeys_.contains(key))
    {
        QDoubleSpinBox::keyPressEvent(event);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


void ICDoubleSpinBox::OnLineEditTextChanged(const QString &text)
{
    qDebug(text.toLatin1());
    lineEdit()->blockSignals(true);
    lineEdit()->setText(QString("%1").arg(text.toInt() / double(decimals() * 10), 0, 'f', decimals()));
    qDebug()<<value();
    lineEdit()->blockSignals(false);
}

void ICDoubleSpinBox::OnFinished()
{
    qDebug("sdfsdf");
}
