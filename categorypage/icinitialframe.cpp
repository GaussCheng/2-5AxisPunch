#include "icinitialframe.h"
#include "ui_icinitialframe.h"

#include "icvirtualhost.h"
#include "iccommandprocessor.h"
#include "icvirtualkey.h"
#include "icactioncommand.h"

ICInitialFrame::ICInitialFrame(QWidget *parent) :
    ICFrame(parent),
    ui(new Ui::ICInitialFrame)
{
    ui->setupUi(this);
    ui->standbyPic->setPixmap(QPixmap("resource/Standby.png"));

//    SetVisibleStatus(false);
}

ICInitialFrame::~ICInitialFrame()
{
    delete ui;
}

void ICInitialFrame::showEvent(QShowEvent *e)
{
    ICFrame::showEvent(e);
//    qDebug("initial page");
//    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
}

void ICInitialFrame::hideEvent(QHideEvent *e)
{
    ICFrame::hideEvent(e);
}

//void ICInitialFrame::on_hideToolButton_clicked()
//{
//    SetVisibleStatus(false);
//}

//void ICInitialFrame::on_enableToolButton_clicked()
//{
//    qDebug("enable");
//}

//void ICInitialFrame::on_showToolButton_clicked()
//{
//    SetVisibleStatus(true);
//}

//void ICInitialFrame::SetVisibleStatus(bool enableStatus)
//{
////    ui->enableToolButton->setVisible(enableStatus);
////    ui->hideToolButton->setVisible(enableStatus);
//}


//void ICInitialFrame::on_pushButton_clicked()
//{
//    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_STOP);
//}

//void ICInitialFrame::on_pushButton_2_clicked()
//{
//     ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnZero, 0);
//}

//void ICInitialFrame::on_pushButton_4_clicked()
//{
//    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_START);
//}


//void ICInitialFrame::on_pushButton_3_clicked()
//{
//    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
//}
