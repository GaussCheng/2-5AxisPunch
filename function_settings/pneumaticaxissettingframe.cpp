#include "pneumaticaxissettingframe.h"
#include "ui_pneumaticaxissettingframe.h"

PneumaticAxisSettingFrame::PneumaticAxisSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PneumaticAxisSettingFrame)
{
    ui->setupUi(this);
}

PneumaticAxisSettingFrame::~PneumaticAxisSettingFrame()
{
    delete ui;
}
