#include "icstandbysettingframe.h"
#include "ui_icstandbysettingframe.h"

ICStandbySettingFrame::ICStandbySettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICStandbySettingFrame)
{
    ui->setupUi(this);
}

ICStandbySettingFrame::~ICStandbySettingFrame()
{
    delete ui;
}
