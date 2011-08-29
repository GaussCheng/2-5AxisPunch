#include "icadminpassworddialog.h"
#include "ui_icadminpassworddialog.h"

ICAdminPasswordDialog::ICAdminPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICAdminPasswordDialog)
{
    ui->setupUi(this);
}

ICAdminPasswordDialog::~ICAdminPasswordDialog()
{
    delete ui;
}

void ICAdminPasswordDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        if(ui->pwdEdit->text() == "szhc2a")
        {
            this->accept();
            return;
        }
    }
    this->reject();
}
