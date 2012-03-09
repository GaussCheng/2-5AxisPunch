#include "icpneumaticactionpage.h"
#include "ui_icpneumaticactionpage.h"
#include "icvirtualhost.h"
#include "icparameterssave.h"

ICPneumaticActionPage::ICPneumaticActionPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICPneumaticActionPage)
{
    QIntValidator * validator = new QIntValidator(0, 32767, this);
    ui->setupUi(this);
    ui->fbDelayLineEdit->SetDecimalPlaces(2);
    ui->fbDelayLineEdit->setValidator(validator);
    ui->udDelayLineEdit->SetDecimalPlaces(2);
    ui->udDelayLineEdit->setValidator(validator);
    ui->posistionDelayLineEdit->SetDecimalPlaces(2);
    ui->posistionDelayLineEdit->setValidator(validator);
}

ICPneumaticActionPage::~ICPneumaticActionPage()
{
    delete ui;
}

void ICPneumaticActionPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICPneumaticActionPage::showEvent(QShowEvent *e)
{
    if(ICParametersSave::Instance()->IsSingleArm())
    {
        ui->subArmGroupBox->hide();
    }
    else
    {
        ui->subArmGroupBox->show();
    }
    ICInstructionEditorBase::showEvent(e);
}

void ICPneumaticActionPage::on_forwardCheckBox_stateChanged(int status)
{
    if(status == Qt::Checked)
    {
        ui->backwardCheckBox->setChecked(false);
    }
}

void ICPneumaticActionPage::on_backwardCheckBox_stateChanged(int status)
{
    if(status == Qt::Checked)
    {
        ui->forwardCheckBox->setChecked(false);
    }
}

void ICPneumaticActionPage::on_upCheckBox_stateChanged(int status)
{
    if(status == Qt::Checked)
    {
        ui->downCheckBox->setChecked(false);
    }
}

void ICPneumaticActionPage::on_downCheckBox_stateChanged(int status)
{
    if(status == Qt::Checked)
    {
        ui->upCheckBox->setChecked(false);
    }
}

QList<ICMoldItem> ICPneumaticActionPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;

    if(ui->subArmGroupBox->isChecked())
    {
        if(ui->forwardCheckBox->isChecked())
        {
            item.SetAction(ICMold::ACTVICEFORWARD);
            item.SetDVal(ui->fbDelayLineEdit->TransThisTextToThisInt());
            ret.append(item);
        }
        else if(ui->backwardCheckBox->isChecked())
        {
            item.SetAction(ICMold::ACTVICEBACKWARD);
            item.SetDVal(ui->fbDelayLineEdit->TransThisTextToThisInt());
            ret.append(item);
        }

        if(ui->upCheckBox->isChecked())
        {
            item.SetAction(ICMold::ACTVICEUP);
            item.SetDVal(ui->udDelayLineEdit->TransThisTextToThisInt());
            ret.append(item);
        }
        else if(ui->downCheckBox->isChecked())
        {
            item.SetAction(ICMold::ACTVICEDOWN);
            item.SetDVal(ui->udDelayLineEdit->TransThisTextToThisInt());
            ret.append(item);
        }
    }

    if(ui->positionGroupBox->isChecked())
    {
        if(ui->horizonRadioButton->isChecked())
        {
            item.SetAction(ICMold::ACTPOSEHORI);
        }
        else
        {
            item.SetAction(ICMold::ACTPOSEVERT);
        }
        item.SetDVal(ui->posistionDelayLineEdit->TransThisTextToThisInt());
        ret.append(item);
    }
    return ret;
}


void ICPneumaticActionPage::on_setButton_clicked()
{
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    host->IsOutputOn(16) ? ui->forwardCheckBox->setChecked(true) : ui->backwardCheckBox->setChecked(true);
    host->IsOutputOn(18) ? ui->upCheckBox->setChecked(true) : ui->downCheckBox->setChecked(true);
    host->IsOutputOn(0) ? ui->horizonRadioButton->setChecked(true) : ui->verticalRadioButton->setChecked(true);
}
