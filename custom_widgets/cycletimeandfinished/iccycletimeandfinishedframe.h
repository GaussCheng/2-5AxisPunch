#ifndef ICCYCLETIMEANDFINISHEDFRAME_H
#define ICCYCLETIMEANDFINISHEDFRAME_H

#include <QFrame>
#include <QPalette>
#include <QTimer>

namespace Ui {
    class ICCycleTimeAndFinishedFrame;
}

class ICAlarmDetailsDialog;

class ICCycleTimeAndFinishedFrame : public QFrame
{
    Q_OBJECT

public:
    ICCycleTimeAndFinishedFrame(QWidget *parent = 0);
//    static ICCycleTimeAndFinishedFrame * Instance()
//    {
//        if(instance_ == NULL)
//        {
//            instance_ = new ICCycleTimeAndFinishedFrame();
//        }
//        return instance_;
//    }
    ~ICCycleTimeAndFinishedFrame();

    void SetCycleTime(const QString & cycleTime);
    void SetFinished(int finishedCount);
    void SetAlarmInfo(const QString & alarmInfo);
    void SetHintInfo(const QString & hintInfo);

protected:
    void changeEvent(QEvent *);
private slots:
    void on_toolButton_clicked();

private:
    void InitInterface();

private:
    Ui::ICCycleTimeAndFinishedFrame *ui;
//    static ICCycleTimeAndFinishedFrame * instance_;
    ICAlarmDetailsDialog *alarmDetailsDialog_;
    QString alarmInfo_;
};

#endif // ICCYCLETIMEANDFINISHEDFRAME_H
