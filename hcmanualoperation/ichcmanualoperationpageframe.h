#ifndef ICHCMANUALOPERATIONPAGEFRAME_H
#define ICHCMANUALOPERATIONPAGEFRAME_H

#include <QFrame>

#include <QMap>

#include "hcmanualadjustframe.h"

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

private:
    Ui::ICHCManualOperationPageFrame *ui;

//    HCServoArmControlFrame * servoArmControlPage_;
//    QWidget * manualFunctionPage_;
    QWidget * manualOtherIOPage_;
    QWidget * manualFixturePage_;
    QWidget * manualSuckerPage_;
    HCManualAdjustFrame * manualAdjustPage_;

    QMap<QWidget *, QWidget *> buttonToPage_;

    QStackedLayout * centralStackedLayout_;
    QButtonGroup* buttonGroup_;
    int currentPose_;
    int currentAction_;
};

#endif // ICHCMANUALOPERATIONPAGEFRAME_H
