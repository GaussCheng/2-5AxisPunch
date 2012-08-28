#include "icalarmframe.h"
#include "ui_icalarmframe.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>

ICAlarmFrame * ICAlarmFrame::instance_ = NULL;

ICAlarmFrame::ICAlarmFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICAlarmFrame),
    alarmString_(ICAlarmString::Instance()),
    AlarmLogFileName("Alarm.log")
{
    ui->setupUi(this);

    ui->alarmHistoryTableWidget->setColumnWidth(2, 200);
    ui->alarmHistoryTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);

    connect(alarmString_,
            SIGNAL(CurrentAlarmNumChanged(int)),
            this,
            SLOT(OnCurrentAlarmChanged(int)));

    connect(alarmString_,
            SIGNAL(CurrentLanguageChanged()),
            this,
            SLOT(OnCurrentLanguageChanged()));

    ReadAlarmInfoInFile();
}

ICAlarmFrame::~ICAlarmFrame()
{
    delete ui;
}

void ICAlarmFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICAlarmFrame::showEvent(QShowEvent *e)
{
    ui->alarmHistoryTableWidget->setRowHeight(ui->alarmHistoryTableWidget->currentRow(), 30);
    ui->alarmHistoryTableWidget->clearSelection();
    QWidget::showEvent(e);
}

void ICAlarmFrame::ReadAlarmInfoInFile()
{
    QFile alarmLogFile(AlarmLogFileName);
    QTextStream alarmTextStream(&alarmLogFile);
    alarmLogFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString alarmFile = alarmTextStream.readAll();
    alarmLogFile.close();
    QStringList alarmItems = alarmFile.split('\n', QString::SkipEmptyParts);
    QStringList optionList;
    int alarmNum;
    for(int i = 0; i != alarmItems.size(); ++i)
    {
        optionList = alarmItems.at(i).split(' ');
        alarmNum = optionList.at(0).toInt();
        QString alarmInfo = alarmString_->AlarmInfo(alarmNum);
        AppendNewLineInTable(alarmNum,
                             optionList[1].replace('_', ' '),
                             alarmInfo);
    }
}

void ICAlarmFrame::AppendNewLineInTable(int currentAlarmNum, const QString & alarmDateTime, const QString & alarmInfo)
{
    QTableWidgetItem * alarmDateTimeItem = new QTableWidgetItem(alarmDateTime);
    QTableWidgetItem * alarmNumItem = new QTableWidgetItem(QString::number(currentAlarmNum));
    QTableWidgetItem * alramInfoItem = new QTableWidgetItem(alarmInfo);

    ui->alarmHistoryTableWidget->insertRow(0);
    ui->alarmHistoryTableWidget->setItem(0, 2, alarmDateTimeItem);
    ui->alarmHistoryTableWidget->setItem(0, 0, alarmNumItem);
    ui->alarmHistoryTableWidget->setItem(0, 1, alramInfoItem);
}

void ICAlarmFrame::RestoreAlarmInfoInLog(int currentAlarmNum, QString alarmDateTime)
{
    QFile alarmLogFile(AlarmLogFileName);
    if(alarmLogFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        QTextStream alarmTextStream(&alarmLogFile);
        alarmTextStream << currentAlarmNum << ' ' << alarmDateTime.replace(' ','_') <<endl;
        alarmLogFile.close();
    }
}
void ICAlarmFrame::OnCurrentAlarmChanged(int currentAlarmNum)
{
    if(ui->alarmHistoryTableWidget->rowCount() > 49)
    {
        ui->alarmHistoryTableWidget->removeRow(ui->alarmHistoryTableWidget->rowCount() - 1);

        QFile alarmLogFile(AlarmLogFileName);
        QTextStream alarmTextStream(&alarmLogFile);

        QString alarms;
        if(alarmLogFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            alarms = alarmTextStream.readAll();
            alarmLogFile.close();
        }
        if(!alarms.isEmpty())
        {
            int firstAlarmIndex = alarms.indexOf("\n");
            alarms.remove(0, firstAlarmIndex + 1);
        }
        if(alarmLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            alarmTextStream<<alarms;
            alarmLogFile.close();
        }
    }

    QString alarmDateTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm");

    AppendNewLineInTable(currentAlarmNum, alarmDateTime,
                         alarmString_->AlarmInfoMap().value(currentAlarmNum));
//    ui->alarmHistoryTableWidget->resizeColumnsToContents();

    RestoreAlarmInfoInLog(currentAlarmNum, alarmDateTime);

    ICAlarmString::Instance()->SetPriorAlarmNum(currentAlarmNum);
}

void ICAlarmFrame::OnCurrentLanguageChanged()
{
    ui->alarmHistoryTableWidget->clearContents();
    ui->alarmHistoryTableWidget->setRowCount(0);
    ReadAlarmInfoInFile();
}

void ICAlarmFrame::on_alarmHistoryTableWidget_clicked(QModelIndex index)
{
    for(int i = 0; i < ui->alarmHistoryTableWidget->rowCount(); ++i)
    {
        if(ui->alarmHistoryTableWidget->rowHeight(i) != 30)
        {
            ui->alarmHistoryTableWidget->setRowHeight(i, 30);
        }
    }

    ui->alarmHistoryTableWidget->setRowHeight(index.row(), 90);;
}
