#include "pneumaticaxiscontrolframe.h"
#include "ui_pneumaticaxiscontrolframe.h"

PneumaticAxisControlFrame::PneumaticAxisControlFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PneumaticAxisControlFrame)
{
    ui->setupUi(this);
}

PneumaticAxisControlFrame::~PneumaticAxisControlFrame()
{
    delete ui;
}
