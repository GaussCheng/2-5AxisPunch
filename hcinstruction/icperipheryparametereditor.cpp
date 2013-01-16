#include "icperipheryparametereditor.h"
#include "ui_icperipheryparametereditor.h"

ICPeripheryParameterEditor::ICPeripheryParameterEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICPeripheryParameterEditor)
{
    ui->setupUi(this);
    /*********BUG#179.180*************/
    QIntValidator *validator = new QIntValidator(0, 255, this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(validator);
    ui->timesEdit->setValidator(validator);
    ui->delayEdit->setText("0.00");
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
