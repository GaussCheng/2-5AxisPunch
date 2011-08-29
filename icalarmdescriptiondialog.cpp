#include "icalarmdescriptiondialog.h"
#include "ui_icalarmdescriptiondialog.h"

ICAlarmDescriptionDialog * ICAlarmDescriptionDialog::instance_ = NULL;

ICAlarmDescriptionDialog::ICAlarmDescriptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICAlarmDescriptionDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

ICAlarmDescriptionDialog::~ICAlarmDescriptionDialog()
{
    delete ui;
}

void ICAlarmDescriptionDialog::SetAlarmDescription(const QString & alarmDescription)
{
    this->ui->alarmInfoLabel->setText(alarmDescription);
    this->exec();
}

void ICAlarmDescriptionDialog::on_okToolButton_clicked()
{
    this->accept();
}
