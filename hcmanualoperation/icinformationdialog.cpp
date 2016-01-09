#include "icinformationdialog.h"
#include "ui_icinformationdialog.h"

#include <QKeyEvent>

#include "mainframe.h"

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

void ICInformationDialog::keyPressEvent(QKeyEvent *e)
{
//    e->ignore();
    QKeyEvent* ke = new QKeyEvent(*e);
    qApp->postEvent(icMainFrame, ke);

    this->close();
}
