#include "icsecuritypointsettingframe.h"
#include "ui_icsecuritypointsettingframe.h"

ICSecurityPointSettingFrame::ICSecurityPointSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICSecurityPointSettingFrame)
{
    ui->setupUi(this);
}

ICSecurityPointSettingFrame::~ICSecurityPointSettingFrame()
{
    delete ui;
}
