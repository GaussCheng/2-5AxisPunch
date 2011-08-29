#ifndef ICSERVOSETTINGFRAME_H
#define ICSERVOSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ICServoSettingFrame;
}

class ICServoSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICServoSettingFrame(QWidget *parent = 0);
    ~ICServoSettingFrame();

private:
    Ui::ICServoSettingFrame *ui;
};

#endif // ICSERVOSETTINGFRAME_H
