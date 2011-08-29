#include "hcpneumaticaxiscontrolframe.h"
#include "ui_hcpneumaticaxiscontrolframe.h"

HCPneumaticAxisControlFrame::HCPneumaticAxisControlFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HCPneumaticAxisControlFrame)
{
    ui->setupUi(this);
}

HCPneumaticAxisControlFrame::~HCPneumaticAxisControlFrame()
{
    delete ui;
}
