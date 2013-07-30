#include "simulateknob.h"
#include "ui_simulateknob.h"

SimulateKnob::SimulateKnob(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulateKnob)
{
    ui->setupUi(this);
    connect(ui->manualButton,
            SIGNAL(clicked()),
            SIGNAL(manualButtonClicked()));
    connect(ui->stopButton,
            SIGNAL(clicked()),
            SIGNAL(stopButtonClicked()));
    connect(ui->autoButton,
            SIGNAL(clicked()),
            SIGNAL(autoButtonClicked()));
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
}

SimulateKnob::~SimulateKnob()
{
    delete ui;
}
