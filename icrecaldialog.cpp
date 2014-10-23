
#include "icrecaldialog.h"
#include "ui_icrecaldialog.h"
#include <QKeyEvent>

ICRecalDialog::ICRecalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICRecalDialog)
{
    ui->setupUi(this);
}

ICRecalDialog::~ICRecalDialog()
{
    delete ui;
}

void ICRecalDialog::changeEvent(QEvent *e)
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

void ICRecalDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_M://No
        this->reject();
        break;
    case Qt::Key_H://Yes
        ::system("touch /home/szhc/recal && sync && reboot");
        this->accept();
        break;
    default:
        break;
    }
}
