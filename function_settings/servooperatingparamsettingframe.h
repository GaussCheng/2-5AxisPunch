#ifndef SERVOOPERATINGPARAMSETTINGFRAME_H
#define SERVOOPERATINGPARAMSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ServoOperatingParamSettingFrame;
}

class ServoOperatingParamSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ServoOperatingParamSettingFrame(QWidget *parent = 0);
    ~ServoOperatingParamSettingFrame();

private:
    Ui::ServoOperatingParamSettingFrame *ui;
};

#endif // SERVOOPERATINGPARAMSETTINGFRAME_H
