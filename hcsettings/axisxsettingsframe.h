#ifndef AXISXSETTINGSFRAME_H
#define AXISXSETTINGSFRAME_H

#include <QFrame>

namespace Ui {
    class AxisXSettingsFrame;
}

class AxisXSettingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AxisXSettingsFrame(QWidget *parent = 0);
    ~AxisXSettingsFrame();

private:
    Ui::AxisXSettingsFrame *ui;
};

#endif // AXISXSETTINGSFRAME_H
