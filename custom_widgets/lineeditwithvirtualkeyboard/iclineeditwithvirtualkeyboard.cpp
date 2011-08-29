#include "iclineeditwithvirtualkeyboard.h"

#include "virtualkeyboarddialog.h"

ICLineEditWithVirtualKeyboard::ICLineEditWithVirtualKeyboard(QWidget * parent)
    : QLineEdit(parent)
{
    this->setReadOnly(true);
    virtualKeyboardDialog_ = VirtualKeyboardDialog::Instance();

    //virtualKeyboardDialog->ResetDisplay(this->text());
}

ICLineEditWithVirtualKeyboard::~ICLineEditWithVirtualKeyboard()
{
}

void ICLineEditWithVirtualKeyboard::mouseReleaseEvent(QMouseEvent *e)
{
    connect(virtualKeyboardDialog_,
            SIGNAL(EnterComplete(QString)),
            this,
            SLOT(SetCurrentText(QString)));

    virtualKeyboardDialog_->ResetDisplay();

    virtualKeyboardDialog_->exec();
    disconnect(virtualKeyboardDialog_,
               SIGNAL(EnterComplete(QString)),
               this,
               SLOT(SetCurrentText(QString)));
    QLineEdit::mouseReleaseEvent(e);
}

void ICLineEditWithVirtualKeyboard::SetCurrentText(const QString &currentText)
{
    this->setText(currentText);
}
