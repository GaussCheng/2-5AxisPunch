#include <QMessageBox>
#include "passworddialog.h"
#include "ui_passworddialog.h"
#include "icparameterssave.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_buttonBox_accepted()
{
    if(ICParametersSave::Instance()->VerifyPassword(static_cast<ICParametersSave::OperationLevel>(ui->levelComboBox->currentIndex()), ui->pwdEdit->text()))
    {
        emit LevelChanged(ui->levelComboBox->currentIndex());
        ui->pwdEdit->clear();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Password is wrong!"));
    }
}

void PasswordDialog::on_levelComboBox_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        ui->pwdEdit->setEnabled(false);
        break;
    default:
        ui->pwdEdit->setEnabled(true);
    }
}

void PasswordDialog::showEvent(QShowEvent *e)
{
    ui->pwdEdit->clear();
    QDialog::showEvent(e);
}

void PasswordDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}
