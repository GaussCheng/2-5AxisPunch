#include <QIntValidator>
#include "icautorunrevise.h"
#include "ui_icautorunrevise.h"
#include "icinstructparam.h"
#include "iccommands.h"
#include "iccommandprocessor.h"

ICAutoRunRevise::ICAutoRunRevise(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICAutoRunRevise)
{
    ui->setupUi(this);

    QIntValidator* validator = new QIntValidator(0, 600, this);
    ui->delayEdit->SetDecimalPlaces(1);
    ui->delayEdit->setValidator(validator);

    validator = new QIntValidator(0, 100, this);
    ui->speedEdit->setValidator(validator);

    validator = new QIntValidator(-50, 50, this);
    ui->posEdit->SetDecimalPlaces(1);
    ui->posEdit->setValidator(validator);
}

ICAutoRunRevise::~ICAutoRunRevise()
{
    delete ui;
}

bool ICAutoRunRevise::ShowModifyItem(const ICMoldItem *item, ICMoldItem* ret, const QString &text)
{
    ui->currentMoldItemLabel->setText(text);
    if(item->IsAction())
    {
        if(item->Action() <= ICMold::GB)
        {
            ui->positionLabel->show();
            ui->posEdit->show();
            ui->speedLabel->show();
            ui->speedEdit->show();
            ui->mmLabel->show();
            ui->precentLabel->show();
        }
    }
    else
    {
        ui->positionLabel->hide();
        ui->posEdit->hide();
        ui->speedLabel->hide();
        ui->speedEdit->hide();
        ui->mmLabel->hide();
        ui->precentLabel->hide();
    }
    ui->posEdit->SetThisIntToThisText(0);
    ui->speedEdit->SetThisIntToThisText(item->SVal());
    ui->delayEdit->SetThisIntToThisText(item->DVal());
    int isok = exec();
    if(isok == QDialog::Accepted)
    {
        ICMoldItem tempItem = *item;
        tempItem.SetPos(tempItem.Pos() + ui->posEdit->TransThisTextToThisInt());
        tempItem.SetDVal(ui->delayEdit->TransThisTextToThisInt());
        tempItem.SetSVal(ui->speedEdit->TransThisTextToThisInt());
        tempItem.ReSum();
        *ret = tempItem;
        ICAutoAdjustCommand command;
        ICCommandProcessor* processor = ICCommandProcessor::Instance();
        command.SetSlave(processor->SlaveID());
        command.SetSequence(tempItem.Seq());
        command.SetDelayTime(tempItem.DVal());
        command.SetSpeed(tempItem.SVal());
        command.SetDPos(ui->posEdit->TransThisTextToThisInt());
        command.SetGMValue(tempItem.GMVal());
        command.SetCheckSum(tempItem.Sum());
        bool isSuccess = processor->ExecuteCommand(command).toBool();
        return isSuccess;
//        if(isSuccess)
//        {
//            item->SetPos(tempItem.Pos());
//            item->SetSVal(tempItem.SVal());
//            item->SetDVal(tempItem.DVal());
//            return true;
//        }
    }
    return false;
}

//void ICAutoRunRevise::on_buttonBox_accepted()
//{
//    int delayTime = ui->delayEdit->TransThisTextToThisInt();
//    int speed = ui->speedEdit->TransThisTextToThisInt();
//    int dx = ui->dxEdit->TransThisTextToThisInt();
//    int dy = ui->dyEdit->TransThisTextToThisInt();
//    int dz = ui->dzEdit->TransThisTextToThisInt();
//    moldItem_->SetDVal(delayTime);
//    moldItem_->SetSVal(speed);
//    moldItem_->SetX(moldItem_->X() + dx);
//    moldItem_->SetY(moldItem_->Y() + dy);
//    moldItem_->SetZ(moldItem_->Z() + dz);
//    moldItem_->ReSum();

//    ICCommandProcessor* processor = ICCommandProcessor::Instance();
//    ICAutoAdjustCommand command;
//    command.SetSlave(processor->SlaveID());
//    command.SetStartAddr(moldItem_->Seq());
//    command.SetCheckSum(moldItem_->Sum());
//    command.SetDelayTime(delayTime);
//    command.SetSpeed(speed);
//    command.SetDx(dx);
//    command.SetDy(dy);
//    command.SetDz(dz);
//    processor->ExecuteCommand(command);
//    emit MoldItemChanged();
//}
