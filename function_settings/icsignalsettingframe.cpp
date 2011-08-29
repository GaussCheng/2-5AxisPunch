#include "icsignalsettingframe.h"
#include "ui_icsignalsettingframe.h"


ICSignalSettingFrame::ICSignalSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICSignalSettingFrame)
{
    ui->setupUi(this);
}

ICSignalSettingFrame::~ICSignalSettingFrame()
{
    delete ui;
}
