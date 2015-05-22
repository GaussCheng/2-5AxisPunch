
#include "icrecaldialog.h"
#include "ui_icrecaldialog.h"
#include "icutility.h"
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
#ifdef Q_WS_QWS
    case Qt::Key_M://No
#else
    case Qt::Key_F4:
#endif
        this->reject();
        break;
#ifdef Q_WS_QWS
    case Qt::Key_H://Yes
#else
    case Qt::Key_F5:
#endif
        ICUtility::system("touch /home/szhc/recal && sync && reboot");
        this->accept();
        break;
    default:
        break;
    }
}
