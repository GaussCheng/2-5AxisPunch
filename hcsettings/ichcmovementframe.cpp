#include "ichcmovementframe.h"
#include "ui_ichcmovementframe.h"

ICHCMovementFrame::ICHCMovementFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCMovementFrame)
{
    ui->setupUi(this);
}

ICHCMovementFrame::~ICHCMovementFrame()
{
    delete ui;
}
