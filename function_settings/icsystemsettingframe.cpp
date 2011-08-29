#include "icsystemsettingframe.h"
#include "ui_icsystemsettingframe.h"

ICSystemSettingFrame::ICSystemSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICSystemSettingFrame)
{
    ui->setupUi(this);
}

ICSystemSettingFrame::~ICSystemSettingFrame()
{
    delete ui;
}
