#include "icalarmframe.h"
#include "ui_icalarmframe.h"
#include "icutility.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QStringList>
#include <QPair>

#define LOG_MAX 500

ICAlarmFrame * ICAlarmFrame::instance_ = NULL;

ICAlarmFrame::ICAlarmFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICAlarmFrame),
    alarmString_(ICAlarmString::Instance()),
    AlarmLogFileName("Alarm.log"),
    clickFlag(true),
    TemporyFileName("temporaryfile.log")
{
    ui->setupUi(this);
    ui->alarmHistoryTableWidget->setColumnWidth(2, 150);
    ui->alarmHistoryTableWidget->setColumnWidth(3, 150);
    ui->alarmHistoryTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);

    connect(alarmString_,
            SIGNAL(CurrentAlarmNumChanged(int)),
            this,
            SLOT(OnCurrentAlarmChanged(int)));

    connect(alarmString_,
            SIGNAL(CurrentLanguageChanged()),
            this,
            SLOT(OnCurrentLanguageChanged()));

    connect(ui->alarmHistoryTableWidget->horizontalHeader(),
            SIGNAL(sectionClicked(int)),
            this,
            SLOT(AlarmTimeSort(int)));

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
//    QFile alarmLogFile(AlarmLogFileName);
//    QTextStream alarmTextStream(&alarmLogFile);
//    alarmLogFile.open(QIODevice::ReadOnly | QIODevice::Text);
//    QString alarmFile = alarmTextStream.readAll();
//    alarmLogFile.close();
////    QStringList alarmItems = alarmFile.split('\n', QString::SkipEmptyParts);
//    alarms_ = alarmFile.split('\n', QString::SkipEmptyParts);
//    QStringList alarmsOrigin = alarms_;
//    QStringList optionList;
//    int alarmNum;
//    QDateTime t = QDateTime::currentDateTime();
//    QString oldDS;
//    QDateTime oldDate;
//    int oldAlarmCount = -1;
//    for(int i = 0; i != alarms_.size(); ++i)
//    {
//        optionList = alarms_.at(i).split(' ');
//        if(optionList.size() != 3) continue;
//        if(oldAlarmCount < 0)
//        {
//            oldDS = optionList.at(1);
//            oldDate = QDateTime::fromString(oldDS, "yyyy-MM-dd_hh:mm");
//            if(oldDate.daysTo(t) <=30) oldAlarmCount = i;
//        }
//        if(optionList.size() == 2)
//        {
//            optionList.append(("no-solve"));
//        }
//        //防止出现报警后未清除报警然后断电，重启系统后报警最后修改时间为乱码的问题
////        else if(optionList.size() == 3 && optionList.at(2).size() != 14)
////        {
////            optionList[2] = QString(tr("no-solve"));
////        }

//        alarmNum = optionList.at(0).toInt();
//        QString alarmInfo = alarmString_->AlarmInfo(alarmNum);
//        AppendNewLineInTable(alarmNum,
//                             optionList[1].replace('_', ' '),
//                             alarmInfo,
//                             optionList[2].replace('_', ' '));
//    }
//    if(oldAlarmCount < 0) oldAlarmCount = alarmsOrigin.size();

//    for(int i = 0; i < oldAlarmCount; ++i)
//    {
//        alarmsOrigin.pop_front();
//    }
//    if(alarmLogFile.open(QFile::WriteOnly | QFile::Text))
//    {
//        alarmLogFile.write(alarmsOrigin.join("\n").toLatin1());
//        alarmLogFile.write("\n");
//        alarmLogFile.close();
//        ICUtility::system("sync");
//    }

    QFile alarmLogFile(AlarmLogFileName);
    QTextStream alarmTextStream(&alarmLogFile);
    alarmLogFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString alarmFile = alarmTextStream.readAll();
    alarmLogFile.close();
//    QStringList alarmItems = alarmFile.split('\n', QString::SkipEmptyParts);
    alarms_ = alarmFile.split('\n', QString::SkipEmptyParts);
    QStringList optionList;
    int alarmNum;
    for(int i = 0; i != alarms_.size(); ++i)
    {
        optionList = alarms_.at(i).split(' ');
        if(optionList.size() < 2) continue;
        if(optionList.size() == 2)
        {
            optionList.append(("no-solve"));
        }
        //防止出现报警后未清除报警然后断电，重启系统后报警最后修改时间为乱码的问题
        else if(optionList.size() == 3 && optionList.at(2).size() != 14)
        {
            optionList[2] = QString(("no-solve"));
        }

        alarmNum = optionList.at(0).toInt();
        QString alarmInfo = alarmString_->AlarmInfo(alarmNum);
        AppendNewLineInTable(alarmNum,
                             optionList[1].replace('_', ' '),
                             alarmInfo,
                             optionList[2].replace('_', ' '));
    }
}

void ICAlarmFrame::AppendNewLineInTable(int currentAlarmNum, const QString & alarmDateTime, const QString & alarmInfo,const QString & alarmModifyTime)
{
    QString resolveTime = alarmModifyTime == "no-solve"? tr("no-solve") : alarmModifyTime;
//    QTableWidgetItem * alarmDateTimeItem = new QTableWidgetItem(alarmDateTime);
//    QTableWidgetItem * alarmNumItem = new QTableWidgetItem(QString::number(currentAlarmNum));
//    QTableWidgetItem * alramInfoItem = new QTableWidgetItem(alarmInfo);
//    QTableWidgetItem * alarmModifyTimeItem = new QTableWidgetItem(resolveTime);

//    ui->alarmHistoryTableWidget->insertRow(0);
//    ui->alarmHistoryTableWidget->setItem(0, 2, alarmDateTimeItem);
//    ui->alarmHistoryTableWidget->setItem(0, 0, alarmNumItem);
//    ui->alarmHistoryTableWidget->setItem(0, 1, alramInfoItem);
//    ui->alarmHistoryTableWidget->setItem(0, 3, alarmModifyTimeItem);
    QTableWidgetItem * alarmDateTimeItem = new QTableWidgetItem(alarmDateTime);
    QTableWidgetItem * alarmNumItem = new QTableWidgetItem(QString::number(currentAlarmNum));
    QTableWidgetItem * alramInfoItem = new QTableWidgetItem(alarmInfo);
    QTableWidgetItem * alarmModifyTimeItem = new QTableWidgetItem(resolveTime);

    ui->alarmHistoryTableWidget->insertRow(0);
    ui->alarmHistoryTableWidget->setItem(0, 2, alarmDateTimeItem);
    ui->alarmHistoryTableWidget->setItem(0, 0, alarmNumItem);
    ui->alarmHistoryTableWidget->setItem(0, 1, alramInfoItem);
    ui->alarmHistoryTableWidget->setItem(0, 3, alarmModifyTimeItem);
}

void ICAlarmFrame::RestoreAlarmInfoInLog(int currentAlarmNum, QString alarmDateTime , QString alarmModifyTime)
{
    QFile alarmLogFile(AlarmLogFileName);
    if(alarmLogFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        QTextStream alarmTextStream(&alarmLogFile);
        alarmTextStream << currentAlarmNum << ' ' << alarmDateTime.replace(' ','_') << ' ' << alarmModifyTime.replace(' ','_') << endl;
        alarmLogFile.close();
        ICUtility::system("sync");
        alarms_.push_back(QString("%1 %2 %3").arg(currentAlarmNum).arg(alarmDateTime.replace(' ','_')).arg(alarmModifyTime.replace(' ','_')));
//        alarmsNoSolve_.append(alarms_.count() - 1);
//        if(alarmsNoSolve_.size() > 50)
//        {
//            alarmsNoSolve_.pop_front();
//        }

//        alarmsNoSolve_.append(alarmPair_(currentAlarmNum , alarms_.count() - 1)) ;
        alarmPair_.first = currentAlarmNum ;
        alarmPair_.second=  alarms_.count() - 1 ;
        alarmsNoSolve_.append(alarmPair_);
        if(alarmsNoSolve_.size() > LOG_MAX)
        {
            alarmsNoSolve_.pop_front();
        }
    }
}
void ICAlarmFrame::OnCurrentAlarmChanged(int currentAlarmNum)
{
//    if(!alarmsNoSolve_.isEmpty())
//    {
//        for(int i = 0 ; i < alarmsNoSolve_.size() ; ++i)
//        {
//            if(currentAlarmNum == alarmsNoSolve_[i].first)
//            {
//                return ;
//            }
//        }
//    }
//    QDateTime t = QDateTime::currentDateTime();
//    QFile alarmLogFile(AlarmLogFileName);
//    QTextStream alarmTextStream(&alarmLogFile);

//    QString alarms;
//    if(alarmLogFile.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        alarms = alarmTextStream.readAll();
//        alarmLogFile.close();
//    }
//    int firstAlarmIndex = alarms.indexOf("\n");
//    if(firstAlarmIndex >= 10)
//    {
//        QString oldDS = alarms.left(firstAlarmIndex).split(" ").at(1);
//        QDateTime oldDate = QDateTime::fromString(oldDS, "yyyy-MM-dd_hh:mm");
//        int days = oldDate.daysTo(t);
//        if(days > 30)
//        {
//            ui->alarmHistoryTableWidget->removeRow(ui->alarmHistoryTableWidget->rowCount() - 1);
//            alarms.remove(0, firstAlarmIndex + 1);
//            alarms_.pop_front();
//            if(!alarmsNoSolve_.isEmpty())
//            {
//                for(int i = 0 ; i < alarmsNoSolve_.size() ; ++i)
//                {
//                    alarmsNoSolve_[i].second-- ;
//                }
//            }
//            if(alarmLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
//            {
//                alarmTextStream<<alarms;
//                alarmLogFile.close();
//                ICUtility::system("sync");
//            }
//        }
//    }
////    if(ui->alarmHistoryTableWidget->rowCount() > 49)
////    {


////        QString alarms;
////        if(alarmLogFile.open(QIODevice::ReadOnly | QIODevice::Text))
////        {
////            alarms = alarmTextStream.readAll();
////            alarmLogFile.close();
////        }
////        if(!alarms.isEmpty())
////        {
////            int firstAlarmIndex = alarms.indexOf("\n");
////            alarms.remove(0, firstAlarmIndex + 1);
////            alarms_.pop_front();
////            if(!alarmsNoSolve_.isEmpty())
////            {
////                for(int i = 0 ; i < alarmsNoSolve_.size() ; ++i)
////                {
////                    alarmsNoSolve_[i].second-- ;
////                }
////            }
////        }
////        if(alarmLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
////        {
////            alarmTextStream<<alarms;
////            alarmLogFile.close();
////        }
////    }
//    QString alarmDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
//    AppendNewLineInTable(currentAlarmNum, alarmDateTime,
//                         alarmString_->AlarmInfoMap().value(currentAlarmNum),tr("no-solve"));

//    RestoreAlarmInfoInLog(currentAlarmNum, alarmDateTime,("no-solve"));

//    ICAlarmString::Instance()->SetPriorAlarmNum(currentAlarmNum);
    if(!alarmsNoSolve_.isEmpty())
       {
           for(int i = 0 ; i < alarmsNoSolve_.size() ; ++i)
           {
               if(currentAlarmNum == alarmsNoSolve_[i].first)
               {
                   return ;
               }
           }
       }
       if(ui->alarmHistoryTableWidget->rowCount() > LOG_MAX - 1)
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
               alarms_.pop_front();
               if(!alarmsNoSolve_.isEmpty())
               {
                   for(int i = 0 ; i < alarmsNoSolve_.size() ; ++i)
                   {
                       alarmsNoSolve_[i].second-- ;
                   }
               }
           }
           if(alarmLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
           {
               alarmTextStream<<alarms;
               alarmLogFile.close();
           }
       }
       QString alarmDateTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm");
       AppendNewLineInTable(currentAlarmNum, alarmDateTime,
                            alarmString_->AlarmInfoMap().value(currentAlarmNum),"no-solve");

       RestoreAlarmInfoInLog(currentAlarmNum, alarmDateTime,("no-solve"));

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

void ICAlarmFrame::AlarmModifyTime()
{
    QString alarmModifyTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm");
    QFile alarmLogFile(AlarmLogFileName);
//    QFile temporaryFile;
//    temporaryFile.setFileName(TemporyFileName);
//    QString line ;
//    if(!temporaryFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
//    {
//        return ;
//    }
    for(int i = 0 ; i < alarmsNoSolve_.size() ; ++i)
    {
        alarms_[alarmsNoSolve_[i].second].replace("no-solve",alarmModifyTime.replace(' ','_'));
    }

    if(alarmLogFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
//        QTextStream alarmTextStream(&alarmLogFile);
//        QTextStream temporaryStream(&temporaryFile);
//        while(!alarmTextStream.atEnd())
//        {
//            line = alarmTextStream.readLine();
//            if(line != "" && line.contains(alarmNum))
//            {
//                line.replace("no-solve",alarmModifytime);
//            }
//            temporaryStream << line << endl ;
//        }
        QByteArray toWrite = alarms_.join("\n").toAscii() + "\n";
        alarmLogFile.write(toWrite);
        alarmLogFile.close();
        ICUtility::system("sync");
        alarmsNoSolve_.clear();
    }
//    temporaryFile.close();
    while(ui->alarmHistoryTableWidget->rowCount() != 0)
    {
        ui->alarmHistoryTableWidget->removeRow(0);
    }
    ReadAlarmInfoInFile();
//    if(!QFile::remove(AlarmLogFileName))
//    {
//        return ;
//    }
//    if(QFile::rename(TemporyFileName , AlarmLogFileName))
//    {
//        ReadAlarmInfoInFile();
//    }
}

void ICAlarmFrame::AlarmTimeSort(int cloumn)
{
    if(cloumn == 2 || cloumn == 3)
    {
        if(clickFlag)
        {
            ui->alarmHistoryTableWidget->sortByColumn(cloumn,Qt::AscendingOrder);
            clickFlag = FALSE ;
        }
        else
        {
            ui->alarmHistoryTableWidget->sortByColumn(cloumn,Qt::DescendingOrder);
            clickFlag = TRUE ;
        }
    }
}
