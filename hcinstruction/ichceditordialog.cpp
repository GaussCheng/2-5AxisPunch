#include "ichceditordialog.h"
#include "ui_ichceditordialog.h"

#include <QIntValidator>

#include "icparameterconversion.h"
#include "icinstructparam.h"

#include <QDebug>

ICHCEditorDialog * ICHCEditorDialog::instance_ = NULL;

ICHCEditorDialog::ICHCEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICHCEditorDialog)
{
    ui->setupUi(this);

    InitInterface();
}

ICHCEditorDialog::~ICHCEditorDialog()
{
    delete ui;
}

void ICHCEditorDialog::changeEvent(QEvent *e)
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

void ICHCEditorDialog::ResetParamInDialog(ICMoldItem moldItem)
{
    moldItem_ = moldItem;
    ui->idLabel->setText(QString::number(moldItem.Seq()));
    ui->numLabel->setText(QString::number(moldItem.Num()));

    uint clipGroup = moldItem.GMVal() & 0x80;
    uint action = moldItem.GMVal() & 0X7F;

    QString actionStr;
    if(!clipGroup)
    {
        actionStr = ICInstructParam::Instance()->ActionGroupMap().value(action);
    }
    else
    {
        actionStr = ICInstructParam::Instance()->ClipGroupMap().value(action);
    }
    ui->actionLabel->setText(actionStr);

    for(int i = 0; i < ui->actionsListWidget->count(); ++ i)
    {
        if(ui->actionsListWidget->item(i)->text() == actionStr)
        {
            ui->actionsListWidget->clearSelection();
            ui->actionsListWidget->item(i)->setSelected(true);
            break;
        }
    }

//    ui->delayTimeLineEdit->SetThisIntToThisText(moldItem.DVal());
//    ui->xLineEdit->SetThisIntToThisText(moldItem.X());
//    ui->yLineEdit->SetThisIntToThisText(moldItem.Y());
//    ui->zLineEdit->SetThisIntToThisText(moldItem.Z());
//    ui->speedLineEdit->SetThisIntToThisText(moldItem.SVal());
}

void ICHCEditorDialog::on_cancelToolButton_clicked()
{
    this->accept();
}

void ICHCEditorDialog::on_okToolButton_clicked()
{
    CommbineCurrentMoldItem();
    emit EnterComplete(moldItem_);
    this->accept();
}

ICMoldItem ICHCEditorDialog::CommbineCurrentMoldItem()
{
//    uint gMVal = 0x0;
//    if(ICInstructParam::Instance()->ActionGroupMap().values().contains(ui->actionLabel->text()))
//    {
//        gMVal |= 0x7f & ICInstructParam::Instance()->ActionGroupMap().key(ui->actionLabel->text());
//    }
//    else
//    {
//        gMVal |= 0x80;
//        gMVal |= 0x7f & ICInstructParam::Instance()->ClipGroupMap().key(ui->actionLabel->text());
//    }
//    moldItem_.SetGMVal(gMVal);

//    moldItem_.SetDVal(ICParameterConversion::TransTextToThisInt(ui->delayTimeLineEdit->text(), 1));
//    moldItem_.SetPos(ICParameterConversion::TransTextToThisInt(ui->xLineEdit->text()
////    moldItem_.SetX(ICParameterConversion::TransTextToThisInt(ui->xLineEdit->text(), 1));
////    moldItem_.SetY(ICParameterConversion::TransTextToThisInt(ui->yLineEdit->text(), 1));
////    moldItem_.SetZ(ICParameterConversion::TransTextToThisInt(ui->zLineEdit->text(), 1));
//    moldItem_.SetSVal(ui->speedLineEdit->text().toInt());
//    moldItem_.ReSum();

//    return moldItem_;
}

void ICHCEditorDialog::InitInterface()
{
    QIntValidator * validator = new QIntValidator(0, 32767, this);
    ui->delayTimeLineEdit->setValidator(validator);
    ui->delayTimeLineEdit->SetDecimalPlaces(1);
    ui->xLineEdit->setValidator(validator);
    ui->xLineEdit->SetDecimalPlaces(1);
    ui->yLineEdit->setValidator(validator);
    ui->yLineEdit->SetDecimalPlaces(1);
    ui->zLineEdit->setValidator(validator);
    ui->zLineEdit->SetDecimalPlaces(1);
    ui->speedLineEdit->setValidator(validator);

    QStringList actions;
    ICInstructParam* instructParam = ICInstructParam::Instance();
//    actions<<instructParam->Action(ICInstructParam::G01)
//           <<instructParam->Action(ICInstructParam::GXY)
//           <<instructParam->Action(ICInstructParam::GXZ)
//           <<instructParam->Action(ICInstructParam::GYX)
//           <<instructParam->Action(ICInstructParam::GYZ)
//           <<instructParam->Action(ICInstructParam::GZX)
//           <<instructParam->Action(ICInstructParam::GZY)
//           <<instructParam->Action(ICInstructParam::ACTVICEUP)
//           <<instructParam->Action(ICInstructParam::ACTVICEDOWN)
//           <<instructParam->Action(ICInstructParam::ACTVICEFORWARD)
//           <<instructParam->Action(ICInstructParam::ACTVICEBACKWARD)
//           <<instructParam->Action(ICInstructParam::ACTPOSEHORI)
//           <<instructParam->Action(ICInstructParam::ACTPOSEVERT)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP1ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP1OFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP2ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP2OFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP3ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP3OFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP4ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP4OFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP5ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP5OFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP6ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP6OFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP7ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP7OFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP8ON)
//           <<instructParam->Clip(ICInstructParam::ACTCLIP8OFF)
//           <<instructParam->Clip(ICInstructParam::ACTEJECTON)
//           <<instructParam->Clip(ICInstructParam::ACTEJECTOFF)
//           <<instructParam->Clip(ICInstructParam::ACTCLSMDON)
//           <<instructParam->Clip(ICInstructParam::ACTCLSMDOFF)
//           <<instructParam->Clip(ICInstructParam::ACTLAYOUTON)
//           <<instructParam->Clip(ICInstructParam::ACTLAYOUTOFF);
    ui->actionsListWidget->addItems(actions);

//    ui->actionsListWidget->addItems(ICInstructParam::Instance()->ActionGroupMap().values());
//    ui->actionsListWidget->addItems(ICInstructParam::Instance()->ClipGroupMap().values());
//    QString action;
//    foreach(action, ICInstructParam::Instance()->ActionGroupMap().values())
//    {
//        AddActionStr(action);
//    }
//    foreach(action, ICInstructParam::Instance()->ClipGroupMap().values())
//    {
//        AddActionStr(action);
//    }

//    ui->actionsListWidget->resizeColumnsToContents();
}

void ICHCEditorDialog::on_actionsListWidget_clicked(QModelIndex index)
{
    ui->actionLabel->setText(ui->actionsListWidget->currentItem()->text());
}

