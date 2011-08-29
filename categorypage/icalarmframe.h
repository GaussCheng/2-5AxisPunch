#ifndef ICALARMFRAME_H
#define ICALARMFRAME_H

#include <QFrame>
#include <QModelIndex>

#include "icalarmstring.h"

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
    void changeEvent(QEvent *);

private slots:
    void on_alarmHistoryTableWidget_clicked(QModelIndex index);
    void OnCurrentLanguageChanged();
    void OnCurrentAlarmChanged(int currentAlarmNum);

private:
    void ReadAlarmInfoInFile();
    void AppendNewLineInTable(int currentAlarmNum, const QString & alarmDateTime, const QString & alarmInfo);
    void RestoreAlarmInfoInLog(int currentAlarmNum, QString alarmDateTime);

private:
    Ui::ICAlarmFrame *ui;
    ICAlarmString *alarmString_;
    static ICAlarmFrame * instance_;
    const QString AlarmLogFileName;
    explicit ICAlarmFrame(QWidget *parent = 0);
};

#endif // ICALARMFRAME_H
