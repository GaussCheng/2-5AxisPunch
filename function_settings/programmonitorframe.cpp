#include "programmonitorframe.h"
#include "ui_programmonitorframe.h"

ProgramMonitorFrame::ProgramMonitorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ProgramMonitorFrame)
{
    ui->setupUi(this);
}

ProgramMonitorFrame::~ProgramMonitorFrame()
{
    delete ui;
}
