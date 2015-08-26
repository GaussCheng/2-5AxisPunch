#include "icinformationdialog.h"
#include "ui_icinformationdialog.h"

ICInformationDialog::ICInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICInformationDialog)
{
    ui->setupUi(this);
//    setModal(true);
}

void ICInformationDialog::setInfo(QString info)
{
    ui->info->setText(info);
}

ICInformationDialog::~ICInformationDialog()
{
    delete ui;
}
