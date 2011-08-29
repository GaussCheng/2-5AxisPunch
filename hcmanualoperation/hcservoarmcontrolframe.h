#ifndef HCSERVOARMCONTROLFRAME_H
#define HCSERVOARMCONTROLFRAME_H

#include <QFrame>

namespace Ui {
    class HCServoArmControlFrame;
}

class HCServoArmControlFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HCServoArmControlFrame(QWidget *parent = 0);
    ~HCServoArmControlFrame();

protected:
    void changeEvent(QEvent *e);
public slots:
    void SetCurrentAxis(const QString & axisName);

private:
    Ui::HCServoArmControlFrame *ui;
    void RunMinus_();
    void RunPlus_();
    void ChangeSpeed_(int dSpeed);

private slots:

private slots:
    void on_plusHighSpeedButton_released();
    void on_plusHighSpeedButton_pressed();
    void on_plusFastButton_released();
    void on_plusFastButton_pressed();
    void on_plusNormalButton_released();
    void on_plusNormalButton_pressed();
    void on_minusHighSpeedButton_released();
    void on_minusHighSpeedButton_pressed();
    void on_minusFastButton_released();
    void on_minusFastButton_pressed();
    void on_minusNormalButton_released();
    void on_minusNormalButton_pressed();
};

#endif // HCSERVOARMCONTROLFRAME_H
