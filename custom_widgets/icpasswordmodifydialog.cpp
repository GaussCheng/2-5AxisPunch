#include "icpasswordmodifydialog.h"
#include "ui_icpasswordmodifydialog.h"
#include <QMessageBox>

ICPasswordModifyDialog::ICPasswordModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICPasswordModifyDialog)
{
    ui->setupUi(this);
}

ICPasswordModifyDialog::~ICPasswordModifyDialog()
{
    delete ui;
}

void ICPasswordModifyDialog::changeEvent(QEvent *e)
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

void ICPasswordModifyDialog::on_okButton_clicked()
{
    if(ui->newPassword->text() != ui->newPassword_2->text())
    {
        QMessageBox::warning(this,
                             tr("warning"),
                             tr("Two password is not the same!"));
        return;
    }
    this->accept();
}

void ICPasswordModifyDialog::on_cancelButton_clicked()
{
    this->reject();
}

QString ICPasswordModifyDialog::OldPassword() const
{
    return ui->oldPassword->text();
}

QString ICPasswordModifyDialog::NewPassword() const
{
    return ui->newPassword->text();
}
