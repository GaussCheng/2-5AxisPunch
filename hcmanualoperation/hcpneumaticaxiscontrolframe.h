#ifndef HCPNEUMATICAXISCONTROLFRAME_H
#define HCPNEUMATICAXISCONTROLFRAME_H

#include <QFrame>

namespace Ui {
    class HCPneumaticAxisControlFrame;
}

class HCPneumaticAxisControlFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HCPneumaticAxisControlFrame(QWidget *parent = 0);
    ~HCPneumaticAxisControlFrame();

private:
    Ui::HCPneumaticAxisControlFrame *ui;
};

#endif // HCPNEUMATICAXISCONTROLFRAME_H
