#ifndef ICALARMFRAME_H
#define ICALARMFRAME_H

#include <QFrame>
#include <QModelIndex>
#include <QTimer>
#include <QPair>
#include "icalarmstring.h"
//#include "iccycletimeandfinishedframe.h"

namespace Ui {
    class ICAlarmFrame;
}

class ICAlarmFrame : public QFrame
{
    Q_OBJECT

public:
    static ICAlarmFrame * Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICAlarmFrame;
        }
        return instance_;
    }
    ~ICAlarmFrame();

protected:
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *);

private slots:
    void on_alarmHistoryTableWidget_clicked(QModelIndex index);
    void OnCurrentLanguageChanged();
    void OnCurrentAlarmChanged(int currentAlarmNum);
    void AlarmTimeSort(int cloumn);
public slots:
    void AlarmModifyTime();

private:
    void ReadAlarmInfoInFile();
    void AppendNewLineInTable(int currentAlarmNum, const QString & alarmDateTime, const QString & alarmInfo ,const QString & alarmModifyTime);
    void RestoreAlarmInfoInLog(int currentAlarmNum, QString alarmDateTime,QString alarmModifyTime);

private:
    Ui::ICAlarmFrame *ui;
    ICAlarmString *alarmString_;
    static ICAlarmFrame * instance_;
    const QString AlarmLogFileName;
    const QString TemporyFileName;
    QStringList alarms_;
    QList<QPair<int, int> > alarmsNoSolve_;
    QPair<int,int>alarmPair_ ;
    bool clickFlag ;

    explicit ICAlarmFrame(QWidget *parent = 0);
};

#endif // ICALARMFRAME_H
