#ifndef SERVOSPEEDSETTINGFRAME_H
#define SERVOSPEEDSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ServoSpeedSettingFrame;
}

class ServoSpeedSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ServoSpeedSettingFrame(QWidget *parent = 0);
    ~ServoSpeedSettingFrame();

private:
    Ui::ServoSpeedSettingFrame *ui;
};

#endif // SERVOSPEEDSETTINGFRAME_H
