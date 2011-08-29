#ifndef ICMANUALOPERATIONPAGEFRAME_H
#define ICMANUALOPERATIONPAGEFRAME_H

#include <QFrame>
#include <QMap>

class ServoArmControlFrame;
class QStackedLayout;

namespace Ui {
    class ICManualOperationPageFrame;
}

class ICManualOperationPageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICManualOperationPageFrame(QWidget *parent = 0);
    ~ICManualOperationPageFrame();

private:
    void InitInterface();
    void InitSignal();

private slots:
    void ShowAxisMovementPage();
    void ShowOptionPage();

private:
    Ui::ICManualOperationPageFrame *ui;

    ServoArmControlFrame * servoArmControlPage_;
    QWidget * manualFunctionPage_;
    QWidget * manualAlterateIOPage_;
    QWidget * manualFixturePage_;
    QWidget * manualSuckerPage_;

    QMap<QWidget *, QWidget *> buttonToPage_;

    QStackedLayout * centralStackedLayout_;
};

#endif // ICMANUALOPERATIONPAGEFRAME_H
