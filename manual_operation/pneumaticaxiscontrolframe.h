#ifndef PNEUMATICAXISCONTROLFRAME_H
#define PNEUMATICAXISCONTROLFRAME_H

#include <QFrame>

namespace Ui {
    class PneumaticAxisControlFrame;
}

class PneumaticAxisControlFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PneumaticAxisControlFrame(QWidget *parent = 0);
    ~PneumaticAxisControlFrame();

private:
    Ui::PneumaticAxisControlFrame *ui;
};

#endif // PNEUMATICAXISCONTROLFRAME_H
