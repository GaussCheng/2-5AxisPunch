#include "axisxsettingsframe.h"
#include "ui_axisxsettingsframe.h"

AxisXSettingsFrame::AxisXSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AxisXSettingsFrame)
{
    ui->setupUi(this);
}

AxisXSettingsFrame::~AxisXSettingsFrame()
{
    delete ui;
}
