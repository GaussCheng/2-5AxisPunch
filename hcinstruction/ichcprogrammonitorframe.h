#ifndef ICHCPROGRAMMONITORFRAME_H
#define ICHCPROGRAMMONITORFRAME_H

#include <QFrame>
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
    void on_editToolButton_clicked();
    void UpdateHostParam();
    void StatusRefreshed();
    void MoldNumChanged(int mold);
    void on_singleStepButton_clicked();

    void on_followToolButton_clicked();

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
    ICAutoRunRevise* autoRunRevise_;
    bool isModify_;
    int oldTime_;
    int newTime_;
    int startIndex_;
    int currentMoldNum_;
    ICGroupMoldUIItem* currentStepItem_;
    bool isFollow_;
    int oldStep_;
};
#endif // ICHCPROGRAMMONITORFRAME_H
