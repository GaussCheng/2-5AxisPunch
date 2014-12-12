#ifndef ICHCPROGRAMMONITORFRAME_H
#define ICHCPROGRAMMONITORFRAME_H

#include <QFrame>
#include <QTimer>
#include <QMap>
#include <QTimer>
#include "icmold.h"

namespace Ui {
    class ICHCProgramMonitorFrame;
}

class ICAutoRunRevise;

class ICHCProgramMonitorFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCProgramMonitorFrame(QWidget *parent = 0);
    ~ICHCProgramMonitorFrame();

public:
    void SetTime(int time);
//    void SetFullTime(int fullTime);
    void SetProduct(int product);
//    void SetCurrentFinished(int currentFinished);


protected:
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private slots:
//    void on_editToolButton_clicked();
    void UpdateHostParam();
    void StatusRefreshed();
    void MoldNumChanged(int mold);
//    void on_singleStepButton_clicked();

//    void on_followToolButton_clicked();

//    void on_speedEnableButton_clicked();

    void OnTimeOut();

//    void on_singleStepButton_pressed();

//    void on_singleStepButton_released();
    void LevelChanged(int);

    void on_cycle_toggled(bool checked);

    void on_xSpeed_toggled(bool checked);

    void on_ySpeed_toggled(bool checked);

#ifdef HC_AXIS_COUNT_5
    void on_zSpeed_toggled(bool checked);
    void on_rSpeed_toggled(bool checked);
    void on_tSpeed_toggled(bool checked);
#endif

private:
    void InitSignal();
    void UpdateUIProgramList_();
    void FindIndex_(int currentIndex, int& groupItemIndex, int& topItemIndex, int& subItemIndex);

//    void AddCommandStr(const QString & commandStr);

public Q_SLOTS:
    void SelectCurrentStep(int currentStep);
    void SubStepChanged(uint8_t* subStep);
    void MoldItemChanged();

private:
    Ui::ICHCProgramMonitorFrame *ui;
    QList<ICGroupMoldUIItem> programList_;
    QList<ICGroupMoldUIItem> programListBackup_;
    ICAutoRunRevise* autoRunRevise_;
    bool isModify_;
    uint oldTime_;
    uint newTime_;
    uint newCycleTimes_;
    uint oldCycleTimes_;
    uint newGoodP_;
    uint oldGoodP_;
    uint newStackedP_;
    uint oldStackedP_;
    int startIndex_;
    ICGroupMoldUIItem* currentStepItem_;
    bool isFollow_;
    int oldStep_;
    int currentMoldNum_;
    QTimer timer_;
    QPixmap switchOn_;
    QPixmap switchOff_;
    QMap<int, int*> fixtureToCountMap_;
    QMap<int, int> checkToFixtureMap_;
    QMap<int, QString> checkResultMap_;
    int fixtureCount_[6];
    QTimer refreshTimer_;
//    QMap<ICMoldItem*, ICMoldItem> modifyMap_;
};
#endif // ICHCPROGRAMMONITORFRAME_H
