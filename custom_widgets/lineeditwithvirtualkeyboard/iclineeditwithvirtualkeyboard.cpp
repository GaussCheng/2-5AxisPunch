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
//    virtualKeyboardDialog_->disconnect();
    connect(virtualKeyboardDialog_,
            SIGNAL(EnterComplete(QString)),
            this,
            SLOT(SetCurrentText(QString)));

    this->setStyleSheet("background:lightgreen;");
    virtualKeyboardDialog_->ResetDisplay();

    virtualKeyboardDialog_->exec();
    disconnect(virtualKeyboardDialog_,
               SIGNAL(EnterComplete(QString)),
               this,
               SLOT(SetCurrentText(QString)));
    QLineEdit::mouseReleaseEvent(e);
    this->setStyleSheet("");
}

void ICLineEditWithVirtualKeyboard::SetCurrentText(const QString &currentText)
{
    this->setText(currentText);
}
