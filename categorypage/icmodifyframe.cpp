#include "icmodifyframe.h"
#include "ui_icmodifyframe.h"
#include "icutility.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QStringList>
#include <QPair>

#define LOG_MAX 500

ICModifyFrame * ICModifyFrame::instance_ = NULL;

ICModifyFrame::ICModifyFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICModifyFrame),
    alarmString_(ICAlarmString::Instance()),
    ModifyLogFileName("Modify.log")
{
    ui->setupUi(this);
    ui->logTable->setColumnWidth(0, 180);
    ui->logTable->setColumnWidth(2, 200);
    ui->logTable->setColumnWidth(3, 200);
    ui->logTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    connect(alarmString_,
            SIGNAL(CurrentLanguageChanged()),
            this,
            SLOT(OnCurrentLanguageChanged()));

    QFile modifyLogFile(ModifyLogFileName);
    QTextStream ts(&modifyLogFile);
    if(modifyLogFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString log = ts.readAll();
        modifyLogFile.close();
        QStringList logs = log.split('\n', QString::SkipEmptyParts);
        QStringList logItems;
        for(int i = 0; i != logs.size(); ++i)
        {
            logItems = logs.at(i).split(',');
            if(logItems.size() != 4)
                continue;

            AppendNewLogInTable(logItems.at(0),
                                logItems.at(1).toUInt(),
                                logItems.at(2),
                                logItems.at(3));
        }
    }

}

ICModifyFrame::~ICModifyFrame()
{
    delete ui;
}

void ICModifyFrame::changeEvent(QEvent *e)
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

void ICModifyFrame::showEvent(QShowEvent *e)
{
    ui->logTable->setRowHeight(ui->logTable->currentRow(), 30);
    ui->logTable->clearSelection();
    QWidget::showEvent(e);
}


void ICModifyFrame::OnCurrentLanguageChanged()
{
    ui->logTable->clearContents();
    ui->logTable->setRowCount(0);
    QFile modifyLogFile(ModifyLogFileName);
    QTextStream ts(&modifyLogFile);
    if(modifyLogFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString log = ts.readAll();
        modifyLogFile.close();
        QStringList logs = log.split('\n', QString::SkipEmptyParts);
        QStringList logItems;
        for(int i = 0; i != logs.size(); ++i)
        {
            logItems = logs.at(i).split(',');
            if(logItems.size() != 4)
                continue;

            AppendNewLogInTable(logItems.at(0),
                                logItems.at(1).toUInt(),
                                logItems.at(2),
                                logItems.at(3));
        }
    }
}

void ICModifyFrame::on_logTable_clicked(QModelIndex index)
{
    for(int i = 0; i < ui->logTable->rowCount(); ++i)
    {
        if(ui->logTable->rowHeight(i) != 30)
        {
            ui->logTable->setRowHeight(i, 30);
        }
    }

    ui->logTable->setRowHeight(index.row(), 90);;
}


void ICModifyFrame::AppendNewLogInTable(const QString &dt, int configID, const QString &newVal, const QString &oldVa)
{
    QTableWidgetItem * modifyDateTimeItem = new QTableWidgetItem(dt);
    modifyDateTimeItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QTableWidgetItem * property = new QTableWidgetItem(ICConfigString::ConfigString(configID));
    property->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QTableWidgetItem * oldValue = new QTableWidgetItem(oldVa);
    oldValue->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QTableWidgetItem * newValue = new QTableWidgetItem(newVal);
    newValue->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


    ui->logTable->insertRow(0);
    ui->logTable->setItem(0, 0, modifyDateTimeItem);
    ui->logTable->setItem(0, 1, property);
    ui->logTable->setItem(0, 2, oldValue);
    ui->logTable->setItem(0, 3, newValue);
}

void ICModifyFrame::OnActionTriggered(int configNum, const QString &newVal, const QString &oldVal)
{
    QFile modifyLogFile(ModifyLogFileName);
    QTextStream ts(&modifyLogFile);
    QString logs;
    if(modifyLogFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        logs = ts.readAll();
    }
    if(ui->logTable->rowCount() > LOG_MAX - 1)
    {
        ui->logTable->removeRow(ui->logTable->rowCount() - 1);

        if(!logs.isEmpty())
        {
            int firstAlarmIndex = logs.indexOf("\n");
            logs.remove(0, firstAlarmIndex + 1);
        }
    }
    logs.append(QString("%1,%2,%3,%4\n").arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss"))
                .arg(configNum)
                .arg(newVal)
                .arg(oldVal));
    modifyLogFile.reset();
    ts<<logs;
    modifyLogFile.close();
    AppendNewLogInTable(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss"),
                        configNum,
                        newVal,
                        oldVal);
    ::system("sync");

}
