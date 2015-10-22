#include "icalarmdetailsdialog.h"
#include "ui_icalarmdetailsdialog.h"
#include <QSqlQuery>

ICAlarmDetailsDialog::ICAlarmDetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICAlarmDetailsDialog)
{
    ui->setupUi(this);
}

ICAlarmDetailsDialog::~ICAlarmDetailsDialog()
{
    delete ui;
}

void ICAlarmDetailsDialog::changeEvent(QEvent *e)
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

void ICAlarmDetailsDialog::ShowDetails(int alarmID)
{
    if(alarmID == 0) return;
    QSqlQuery query;
    query.exec(QString("SELECT * FROM tb_alarm_helper_zh_cn WHERE alarm_id = %1").arg(alarmID));
    if(query.next())
    {
        ui->alarmID->setText(query.value(0).toString());
        ui->alarmText->setText(query.value(1).toString());
        ui->alarmReason->setText(query.value(2).toString());
        ui->alarmSolution->setText(query.value(3).toString());
        this->show();
    }
}

void ICAlarmDetailsDialog::on_closeButton_clicked()
{
    this->accept();
}
