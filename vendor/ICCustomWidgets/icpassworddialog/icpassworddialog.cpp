#include "icpassworddialog.h"
#include "ui_icpassworddialog.h"
#include <QMessageBox>
#include <QKeyEvent>

ICPasswordDialog::ICPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICPasswordDialog),
    currentFocusRow_(0),
    currentFocusColumn_(0)
{
    ui->setupUi(this);       
    eventLoop_ = new QEventLoop;
    widgets_<<(QList<QWidget*>()<<ui->firstPassword)
           <<(QList<QWidget*>()<<ui->secPassword)
          <<(QList<QWidget*>()<<ui->okButton<<ui->cancelButton);
    ui->firstPassword->installEventFilter(this);
    ui->secPassword->installEventFilter(this);
    ui->okButton->installEventFilter(this);
    ui->cancelButton->installEventFilter(this);

    ui->firstPassword->SetEchoMode(ICInputMethodEdit::EchoMode_PasswordAfter);
    ui->secPassword->SetEchoMode(ICInputMethodEdit::EchoMode_PasswordAfter);
    ui->firstPassword->setFocus();
    setWindowModality(Qt::WindowModal);

}

ICPasswordDialog::~ICPasswordDialog()
{
    delete ui;
    delete eventLoop_;
}

void ICPasswordDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICPasswordDialog::showEvent(QShowEvent *e)
{       
    currentFocusRow_ = 0;
    currentFocusColumn_ = 0;
    ui->firstPassword->setFocus();
    QDialog::showEvent(e);
}

bool ICPasswordDialog::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        int key = keyEvent->key();
        QWidget* w = NULL;
        if(key == Qt::Key_Left ||
                key == Qt::Key_Right ||
                key == Qt::Key_Up ||
                key == Qt::Key_Down)
        {
            if(key == Qt::Key_Right)
            {
                if(currentFocusColumn_ != widgets_.at(currentFocusRow_).size() - 1)
                {
                    ++currentFocusColumn_;
                }
                else
                {
                    currentFocusColumn_ = 0;
                    currentFocusRow_ = (currentFocusRow_ + 1) % widgets_.size();
                }
            }
            else if(key == Qt::Key_Left)
            {
                if(currentFocusColumn_ > 0)
                {
                    --currentFocusColumn_;
                }
                else
                {
                    --currentFocusRow_;
                    if(currentFocusRow_ < 0)
                    {
                        currentFocusRow_ = (widgets_.size() + currentFocusRow_);
                    }
                    currentFocusColumn_ = widgets_.at(currentFocusRow_).size() - 1;
                }
            }
            else if(key == Qt::Key_Up)
            {
                --currentFocusRow_;
                if(currentFocusRow_ < 0)
                {
                    currentFocusRow_ = (widgets_.size() + currentFocusRow_);
                }
                if(currentFocusColumn_ > widgets_.at(currentFocusRow_).size() - 1)
                {
                    currentFocusColumn_ = widgets_.at(currentFocusRow_).size() - 1;
                }
            }
            else if(key == Qt::Key_Down)
            {
                currentFocusRow_ = (currentFocusRow_ + 1) % widgets_.size();
                if(currentFocusColumn_ > widgets_.at(currentFocusRow_).size() - 1)
                {
                    currentFocusColumn_ = widgets_.at(currentFocusRow_).size() - 1;
                }
            }
            w = widgets_[currentFocusRow_][currentFocusColumn_];
            if(w != NULL)
            {
                widgets_[currentFocusRow_][currentFocusColumn_]->setFocus();
                return true;
            }
        }

    }
    return QWidget::eventFilter(o, e);
}
void ICPasswordDialog::on_okButton_clicked()
{
    if(ui->firstPassword->text().isEmpty() || ui->secPassword->text().isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Password can not be empty!"));
        return;
    }
    if(ui->firstPassword->ActualText() == ui->secPassword->ActualText())
    {
        passord_ = ui->firstPassword->ActualText();
        accept();
    }
    else
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Password is not the same!"));
    }
    eventLoop_->quit();

}

void ICPasswordDialog::on_cancelButton_clicked()
{
    reject();
    eventLoop_->quit();
}

void ICPasswordDialog::SetTips(const QString &tips)
{
    ui->tips->setText(tips);
}

void ICPasswordDialog::EnterEventLoop()
{
    eventLoop_->exec();
}
