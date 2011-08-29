#include "servooperatingparamsettingframe.h"
#include "ui_servooperatingparamsettingframe.h"

ServoOperatingParamSettingFrame::ServoOperatingParamSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ServoOperatingParamSettingFrame)
{
    ui->setupUi(this);
}

ServoOperatingParamSettingFrame::~ServoOperatingParamSettingFrame()
{
    delete ui;
}
