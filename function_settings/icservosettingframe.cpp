#include "icservosettingframe.h"
#include "ui_icservosettingframe.h"

ICServoSettingFrame::ICServoSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICServoSettingFrame)
{
    ui->setupUi(this);
}

ICServoSettingFrame::~ICServoSettingFrame()
{
    delete ui;
}
