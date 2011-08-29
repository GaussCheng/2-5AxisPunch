#ifndef SERVOARMCONTROLFRAME_H
#define SERVOARMCONTROLFRAME_H

#include <QFrame>

namespace Ui {
    class ServoArmControlFrame;
}

class ServoArmControlFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ServoArmControlFrame(QWidget *parent = 0);
    ~ServoArmControlFrame();

public slots:
    void SetCurrentAxis(const QString & axisName);

private:
    Ui::ServoArmControlFrame *ui;
};

#endif // SERVOARMCONTROLFRAME_H
