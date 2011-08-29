#include "servospeedsettingframe.h"
#include "ui_servospeedsettingframe.h"

ServoSpeedSettingFrame::ServoSpeedSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ServoSpeedSettingFrame)
{
    ui->setupUi(this);
}

ServoSpeedSettingFrame::~ServoSpeedSettingFrame()
{
    delete ui;
}
