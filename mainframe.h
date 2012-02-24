#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QWidget>
#include <QMap>
#include <QSplashScreen>
#include <QScopedPointer>
#include "icscreensaver.h"
#include "icfunctionpagebackframe.h"

class QStackedLayout;
class QDialog;
class QButtonGroup;
class QAbstractButton;

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

//    void SetBackLightOff();
//    void SetBackLightOn();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);

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
    ICFunctionPageBackFrame * functionPage_;
    QWidget * alarmPage_;
    QWidget * recordPage_;
    QWidget * monitorPage_;
    QWidget * instructPage_;
    QWidget * autoPage_;
//    QWidget * returnPage_;
    QWidget * manualPage_;
    QDialog *originExecutingPage_;
    QDialog *returnExecutingPage_;
    QDialog *actionDialog_;

    QMap <QWidget *, QWidget *> functionButtonToPage_;
    QStackedLayout * centerStackedLayout_;
    QButtonGroup* buttonGroup_;
    QAbstractButton* nullButton_;

    int ledFD_;
    int ledFlags_;
    int newLedFlags_;
    uint errCode_;
    int runningStatus_;
    int oldRunnigStatus_;
    int oldXPos_;
    int oldYPos_;
    int oldZPos_;
    int oldX2Pos_;
    int oldY2Pos_;
    int oldAPos_;
    int oldBPos_;
    int oldCPos_;
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
    bool isX2PosChanged_;
    bool isY2PosChanged_;
    bool isAPosChanged_;
    bool isBPosChanged_;
    bool isCPosChanged_;
    int axisDefine_;
    int timerID_;
    QList<QList<QWidget*> > axisWidgets_;
};

extern MainFrame* icMainFrame;

#endif // MAINFRAME_H
