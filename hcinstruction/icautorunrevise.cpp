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

    validator = new QIntValidator(0, 30000, this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(validator);

    returnStepValidator = new QIntValidator(1, 1000, this);

    QIntValidator* spValidator = new QIntValidator(0, 100, this);
    ui->speedEdit->setValidator(spValidator);

    QIntValidator* posValidator = new QIntValidator(-50, 50, this);
    ui->posEdit->SetDecimalPlaces(1);
    ui->posEdit->setValidator(posValidator);

    ui->delayEdit->SetModalKeyboard(true);
    ui->speedEdit->SetModalKeyboard(true);
    ui->posEdit->SetModalKeyboard(true);
}

ICAutoRunRevise::~ICAutoRunRevise()
{
    delete ui;
}

void ICAutoRunRevise::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
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

bool ICAutoRunRevise::ShowModifyItem(const ICMoldItem *item, ICMoldItem* ret, const QString &text)
{
    qDebug("In show editor");
    qDebug()<<(item == NULL);
    qDebug()<<(ret == NULL);

    ui->currentMoldItemLabel->setText(text);
    ui->positionLabel->hide();
    ui->posEdit->hide();
    ui->speedLabel->hide();
    ui->speedEdit->hide();
    ui->mmLabel->hide();
    ui->precentLabel->hide();

    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(validator);
    ui->label->setText(tr("Delay Time:"));
    ui->sLabel->show();

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
        else if(item->Action() == ICMold::ACTCHECKINPUT)
        {
            ui->label->setText(tr("Return Step:"));
            ui->delayEdit->SetDecimalPlaces(0);
            ui->delayEdit->setValidator(returnStepValidator);
            ui->sLabel->hide();
        }
    }
    ui->posEdit->SetThisIntToThisText(0);
    ui->speedEdit->SetThisIntToThisText(item->SVal());
    ui->delayEdit->SetThisIntToThisText(item->DVal());
    int isok = exec();
    if(isok == QDialog::Accepted)
    {
        ret->SetPos(ui->posEdit->TransThisTextToThisInt());
        ret->SetDVal(ui->delayEdit->TransThisTextToThisInt());
        ret->SetSVal(ui->speedEdit->TransThisTextToThisInt());
//        ICMoldItem tempItem = *item;

//        tempItem.SetPos(tempItem.Pos() + ui->posEdit->TransThisTextToThisInt());
//        if(tempItem.Pos() < 0)
//        {
//            tempItem.SetPos(0);
//        }
//        tempItem.SetDVal(ui->delayEdit->TransThisTextToThisInt());
//        tempItem.SetSVal(ui->speedEdit->TransThisTextToThisInt());
//        tempItem.ReSum();
//        *ret = tempItem;

//        ICAutoAdjustCommand command;
//        ICCommandProcessor* processor = ICCommandProcessor::Instance();
//        command.SetSlave(processor->SlaveID());
//        command.SetSequence(tempItem.Seq());
//        command.SetDelayTime(tempItem.DVal());
//        command.SetSpeed(tempItem.SVal());
//        command.SetDPos(ui->posEdit->TransThisTextToThisInt());
//        command.SetGMValue(tempItem.GMVal());
//        command.SetCheckSum(tempItem.Sum());
//        bool isSuccess = processor->ExecuteCommand(command).toBool();
//#ifdef Q_WS_X11
        return true;
//#endif
        //return isSuccess;
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
