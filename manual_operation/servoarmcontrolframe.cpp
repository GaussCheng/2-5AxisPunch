#include "servoarmcontrolframe.h"
#include "ui_servoarmcontrolframe.h"

ServoArmControlFrame::ServoArmControlFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ServoArmControlFrame)
{
    ui->setupUi(this);
}

ServoArmControlFrame::~ServoArmControlFrame()
{
    delete ui;
}

void ServoArmControlFrame::SetCurrentAxis(const QString & axisName)
{
    ui->currentServoArmLabel->setText(axisName);
}
