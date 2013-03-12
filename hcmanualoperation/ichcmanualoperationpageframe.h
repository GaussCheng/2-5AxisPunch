#ifndef ICHCMANUALOPERATIONPAGEFRAME_H
#define ICHCMANUALOPERATIONPAGEFRAME_H

#include <QFrame>

#include <QMap>

#include "hcmanualadjustframe.h"
#include "hcservoarmcontrolframe.h"

class HCServoArmControlFrame;
class QStackedLayout;
class QButtonGroup;

namespace Ui {
    class ICHCManualOperationPageFrame;
}

class ICHCManualOperationPageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCManualOperationPageFrame(QWidget *parent = 0);
    void AdjustFrameTransfer();
    ~ICHCManualOperationPageFrame();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);
private:
    void InitInterface();
    void InitSignal();

private slots:
//    void ShowAxisMovementPage();
    void ShowOptionPage();
    void StatusRefreshed();

    void on_aAddButton_pressed();

    void OnButtonReleased();

    void on_aSubButton_pressed();

    void on_bSubButton_pressed();

    void on_bAddButton_pressed();

private:
    Ui::ICHCManualOperationPageFrame *ui;

//    HCServoArmControlFrame * servoArmControlPage_;
//    QWidget * manualFunctionPage_;
    QWidget *manualOtherIOPage_;
    QWidget *manualFixturePage_;
    QWidget *manualSuckerPage_;
    HCManualAdjustFrame * manualAdjustPage_;
    QWidget *reservePage_;
    HCServoArmControlFrame *serveAxisPage_;

    QMap<QWidget *, QWidget *> buttonToPage_;
    QMap<QWidget*, int> buttonToAxis_;

    QStackedLayout * centralStackedLayout_;
    QButtonGroup* buttonGroup_;
    int currentAction_;
    int timerID_;
    QString positionText_;
};

#endif // ICHCMANUALOPERATIONPAGEFRAME_H
