#ifndef PNEUMATICAXISSETTINGFRAME_H
#define PNEUMATICAXISSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class PneumaticAxisSettingFrame;
}

class PneumaticAxisSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PneumaticAxisSettingFrame(QWidget *parent = 0);
    ~PneumaticAxisSettingFrame();

private:
    Ui::PneumaticAxisSettingFrame *ui;
};

#endif // PNEUMATICAXISSETTINGFRAME_H
