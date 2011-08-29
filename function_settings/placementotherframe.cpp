#include "placementotherframe.h"
#include "ui_placementotherframe.h"

PlacementOtherFrame::PlacementOtherFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlacementOtherFrame)
{
    ui->setupUi(this);
}

PlacementOtherFrame::~PlacementOtherFrame()
{
    delete ui;
}
