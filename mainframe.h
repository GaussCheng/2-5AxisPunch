#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QWidget>
#include <QMap>
#include <QSplashScreen>
#include <QScopedPointer>
#include "icscreensaver.h"
#include "ichcmanualoperationpageframe.h"
#include "ichcprogrammonitorframe.h"
#include "icsettingsframe.h"


class QStackedLayout;
class QDialog;
class QButtonGroup;
class QAbstractButton;
class ICHCInstructionPageFrame;
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
class SimulateKnob;
#endif

namespace Ui {
    class MainFrame;
}

class MainFrame : public QWidget {
    Q_OBJECT
public:
    MainFrame(QSplashScreen *splashScreen, QWidget *parent = 0);
    ~MainFrame();

    bool IsOrigin() const { return isOriginShown_;}
    bool IsOrigined() const;

    bool IsInput() const;
    void SetHasInput(bool isInput);
    bool IsBackLightOff() const;
    void SetBackLightOff(bool isOff);
    void ShowScreenSaver();
    bool IsAutoPageShown() const {if(autoPage_ == NULL) return false; return !autoPage_->isHidden();}

    bool NoInStop();

public Q_SLOTS:
    void StatusRefreshed();
    void ShowManualPage();
    void ShowAutoPage();
    void ShowInstructPage();
    void ShowStandbyPage();
    void ShowOrigin();
    void HideOrigin();
    void ShowReturn();
    void HideReturn();
    void UpdateAxisDefine_();
    void KeyToInstructEditor(int key);
    void ClearPosColor();

//    void SetBackLightOff();
//    void SetBackLightOn();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);

Q_SIGNALS:
    void LoadMessage(const QString&);

private slots:
    void CategoryButtonClicked();
    void ReturnButtonClicked();
    void ShowFunctionPage();
    void LevelChanged(int level);
    void StepChanged(int step);
    void RecordButtonClicked();
    void CheckedInput();
    void checkAlarmModify();
    void CountRestTime();

//    void InitHeavyPage();

private:
    void InitCategoryPage();
    void InitInterface();
    void InitSignal();


    void UpdateTranslate();
    void ShowWidgets_(QList<QWidget*>& widgets);
    void HideWidgets_(QList<QWidget*> & widgets);

private:

    Ui::MainFrame *ui;

    QWidget * initialPage_;
    ICSettingsFrame * settingsPage_;
    QWidget * alarmPage_;
    QWidget * recordPage_;
    QWidget * monitorPage_;
    ICHCInstructionPageFrame * instructPage_;
    QWidget * autoPage_;
//    QWidget * returnPage_;
    ICHCManualOperationPageFrame * manualPage_;
    QDialog *originExecutingPage_;
    QDialog *returnExecutingPage_;
    QDialog *actionDialog_;
    QDialog *hostCompareDialog_;

    QMap <QWidget *, QWidget *> functionButtonToPage_;
    QStackedLayout * centerStackedLayout_;
    QButtonGroup* buttonGroup_;
    QAbstractButton* nullButton_;
    QTimer *registe_timer;
    QTimer *reboot_timer;
    int resetTime;

    int ledFD_;
    int ledFlags_;
    int newLedFlags_;
    uint errCode_;
    int runningStatus_;
    int oldRunnigStatus_;
    int oldXPos_;
    int oldYPos_;
    int oldZPos_;
    QString speed_;
    QString statusStr_;
    bool isOriginShown_;
    bool isReturnShown_;
    int finishCount_;
    int oldFinishCount_;
    int cycleTime_;
    int oldCycleTime_;
    int oldStep_;

    QScopedPointer<ICScreenSaver> screenSaver_;
    bool isBackLightOff_;
    bool isOrigined_;
    bool isDoAction_;
    QPoint mousePoint_;
    int oldHintCode_;
    bool isXPosChanged_;
    bool isYPosChanged_;
    bool isZPosChanged_;
    int axisDefine_;
    int timerID_;

    bool isOverTime_;
    QList<QList<QWidget*> > axisWidgets_;
    QList<uint> compareAlarmNums_;
    QTimer timer_;
#ifdef HC_SK_8_SC
    int oldSw_;
    int oldKey_;
#endif
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    SimulateKnob* simulateKnob_;
#endif

};

extern MainFrame* icMainFrame;

#endif // MAINFRAME_H
