#include "icperipheryparametereditor.h"
#include "ui_icperipheryparametereditor.h"

ICPeripheryParameterEditor::ICPeripheryParameterEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICPeripheryParameterEditor)
{
    ui->setupUi(this);
    QIntValidator *validator = new QIntValidator(0, 1000, this);
    ui->delayEdit->SetDecimalPlaces(1);
    ui->delayEdit->setValidator(validator);
    ui->timesEdit->setValidator(validator);
}

ICPeripheryParameterEditor::~ICPeripheryParameterEditor()
{
    delete ui;
}

int ICPeripheryParameterEditor::Delay() const
{
    return ui->delayEdit->TransThisTextToThisInt();
}

int ICPeripheryParameterEditor::Times() const
{
    return ui->timesEdit->TransThisTextToThisInt();
}
