#include "manualfixtureframe.h"
#include "ui_manualfixtureframe.h"

ManualFixtureFrame::ManualFixtureFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ManualFixtureFrame)
{
    ui->setupUi(this);
}

ManualFixtureFrame::~ManualFixtureFrame()
{
    delete ui;
}
