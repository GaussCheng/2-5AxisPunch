#include "operatingratiosetdialog.h"
#include "ui_operatingratiosetdialog.h"

#include "iccommandprocessor.h"
#include "icvirtualkey.h"

OperatingRatioSetDialog * OperatingRatioSetDialog::instance_ = NULL;

OperatingRatioSetDialog::OperatingRatioSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperatingRatioSetDialog)
{
    ui->setupUi(this);
}

OperatingRatioSetDialog::~OperatingRatioSetDialog()
{
    delete ui;
}

void OperatingRatioSetDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

void OperatingRatioSetDialog::on_okToolButton_clicked()
{
    this->accept();
}

OperatingRatioSetDialog::PulleySpeed OperatingRatioSetDialog::CurrentPulleySpeed() const
{
    if(ui->x1RadioButton->isChecked())
    {
        return X1;
    }
    else if(ui->x5RadioButton->isChecked())
    {
        return X5;
    }
    else if(ui->x10RadioButton->isChecked())
    {
        return X10;
    }
    else if(ui->x20RadioButton->isChecked())
    {
        return X20;
    }
    else if(ui->x50RadioButton->isChecked())
    {
        return X50;
    }
    else
    {
        return X5;
    }
}

OperatingRatioSetDialog::GlobalSpeedStep OperatingRatioSetDialog::CurrentGlobalSpeedStep() const
{
    if(ui->gx1RadioButton->isChecked())
    {
        return G_X1;
    }
    else if(ui->gx10RadioButton->isChecked())
    {
        return G_X10;
    }
    else if(ui->gx5RadioButton->isChecked())
    {
        return G_X5;
    }
    else if(ui->gx20RadioButton->isChecked())
    {
        return G_X20;
    }
    else
    {
        return G_X5;
    }
}

void OperatingRatioSetDialog::SetCurrentHandwheelAxis(HandwheelAxis axis)
{
    switch(axis)
    {
    case Axis_X:
        ui->xAxisRadioButton->setChecked(true);
        break;
    case Axis_Y:
        ui->yAxisRadioButton->setChecked(true);
        break;
    case Axis_Z:
        ui->xAxisRadioButton->setChecked(true);
        break;
    default:
        ui->xAxisRadioButton->setChecked(true);
        break;
    }
}

void OperatingRatioSetDialog::on_xAxisRadioButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_BIASX);
}

void OperatingRatioSetDialog::on_yAxisRadioButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_BIASY);
}

void OperatingRatioSetDialog::on_zAxisRadioButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_BIASZ);
}
