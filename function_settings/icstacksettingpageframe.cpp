#include "icstacksettingpageframe.h"
#include "ui_icstacksettingpageframe.h"

ICStackSettingPageFrame::ICStackSettingPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICStackSettingPageFrame)
{
    ui->setupUi(this);
}

ICStackSettingPageFrame::~ICStackSettingPageFrame()
{
    delete ui;
}
