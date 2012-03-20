#include "icstructdefineframe.h"
#include "ui_icstructdefineframe.h"

#include "iccommandprocessor.h"
#include "iccommands.h"
#include "icvirtualhost.h"
#include "icparameterssave.h"

#include <QVector>
#include <QMessageBox>
#include <QDebug>
#include "mainframe.h"

ICStructDefineFrame::ICStructDefineFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICStructDefineFrame)
{
    ui->setupUi(this);
    if(ICParametersSave::Instance()->IsSingleArm())
    {
        ui->singleArmButton->setChecked(true);
        on_doubleArmButton_toggled(false);
//        ui->doubleArmButton->toggle();
//        ui->singleArmButton->click();
    }
    else
    {
        ui->doubleArmButton->click();
//        ui->doubleArmButton->setChecked(true);
////        ui->singleArmButton->setCheckState();
//        on_doubleArmButton_toggled(!ICParametersSave::Instance()->IsSingleArm());
    }
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    armStruct_ = host->SystemParameter(ICVirtualHost::SYS_Config_Signal).toUInt();
    ui->mainArmForwardLimitButton->setChecked(host->HasMainArmForwardLimit());
    ui->mainArmBackwardLimitButton->setChecked(host->HasMainArmBackwardLimit());
    ui->mainArmDownLimitButton->setChecked(host->HasMainArmDownLimit());
    ui->subArmForwardLimitButton->setChecked(host->HasSubArmForwardLimit());
    ui->subArmBackwardLimitButton->setChecked(host->HasSubArmBackwardLimit());
    ui->subArmDownLimitButton->setChecked(host->HasSubArmDownLimit());

    /*axis define*/
    boxToAxis_.insert(ui->x1Box, ICVirtualHost::ICAxis_AxisX1);
    boxToAxis_.insert(ui->y1Box, ICVirtualHost::ICAxis_AxisY1);
    boxToAxis_.insert(ui->x2Box, ICVirtualHost::ICAxis_AxisX2);
    boxToAxis_.insert(ui->y2Box, ICVirtualHost::ICAxis_AxisY2);
    boxToAxis_.insert(ui->zBox, ICVirtualHost::ICAxis_AxisZ);
    boxToAxis_.insert(ui->aBox, ICVirtualHost::ICAxis_AxisA);
    boxToAxis_.insert(ui->bBox, ICVirtualHost::ICAxis_AxisB);
    boxToAxis_.insert(ui->cBox, ICVirtualHost::ICAxis_AxisC);
    armDefineToIndex_.insert(ICVirtualHost::ICAxisDefine_None, 0);
    armDefineToIndex_.insert(ICVirtualHost::ICAxisDefine_Pneumatic, 1);
    armDefineToIndex_.insert(ICVirtualHost::ICAxisDefine_Servo, 2);
    indexToArmDefine_.insert(0, ICVirtualHost::ICAxisDefine_None);
    indexToArmDefine_.insert(1, ICVirtualHost::ICAxisDefine_Pneumatic);
    indexToArmDefine_.insert(2, ICVirtualHost::ICAxisDefine_Servo);
    QList<QComboBox*> boxs = ui->armDefineBox->findChildren<QComboBox*>();
    axisDefine_ = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
    for(int i = 0; i != boxs.size(); ++i)
    {
        boxs[i]->setCurrentIndex(armDefineToIndex_.value(host->AxisDefine(static_cast<ICVirtualHost::ICAxis>(boxToAxis_.value(boxs.at(i))))));
        connect(boxs[i],
                SIGNAL(currentIndexChanged(int)),
                SLOT(OnAxisDefineChanged(int)));
    }

    outputDefineToNumber_.insert(ui->outABox, 0);
    outputDefineToNumber_.insert(ui->outBBox, 1);
    outputDefineToNumber_.insert(ui->outCBox, 2);
    outputDefineToNumber_.insert(ui->outDBox, 3);
    outputDefineToNumber_.insert(ui->outEBox, 4);
    outputDefineToNumber_.insert(ui->outFBox, 5);
    outputDefineToNumber_.insert(ui->outGBox, 6);
    outputDefineToNumber_.insert(ui->outHBox, 7);
    numberToOutputDefine_.insert(0, ui->outABox);
    numberToOutputDefine_.insert(1, ui->outBBox);
    numberToOutputDefine_.insert(2, ui->outCBox);
    numberToOutputDefine_.insert(3, ui->outDBox);
    numberToOutputDefine_.insert(4, ui->outEBox);
    numberToOutputDefine_.insert(5, ui->outFBox);
    numberToOutputDefine_.insert(6, ui->outGBox);
    numberToOutputDefine_.insert(7, ui->outHBox);
    outDefine_ = host->SystemParameter(ICVirtualHost::ICVirtualHost::SYS_Config_Out).toInt();
    boxs = ui->outDefineBox->findChildren<QComboBox*>();
    for(int i = 0; i != boxs.size(); ++i)
    {
        boxs[i]->setCurrentIndex(host->PeripheryOutput(outputDefineToNumber_.value(boxs.at(i))));
        connect(boxs[i],
                SIGNAL(currentIndexChanged(int)),
                SLOT(OnOutputDefineChanged(int)));
    }

    ui->fixtureSelectBox->setCurrentIndex(host->FixtureDefine());
    ui->escapeComboBox->setCurrentIndex(host->EscapeWay());
}

ICStructDefineFrame::~ICStructDefineFrame()
{
    delete ui;
}

void ICStructDefineFrame::changeEvent(QEvent *e)
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


void ICStructDefineFrame::on_saveButton_clicked()
{
    ICSetAxisConfigsCommand command;
    ICCommandProcessor* process = ICCommandProcessor::Instance();
    int sum = 0;
    QVector<uint> dataBuffer(7, 0);
    dataBuffer[0] = armStruct_;
    dataBuffer[1] = axisDefine_;
    dataBuffer[2] = outDefine_;
    dataBuffer[3] = ICVirtualHost::GlobalVirtualHost()->FixtureDefineSwitch(ui->fixtureSelectBox->currentIndex());
    for(int i = 0; i != 6; ++i)
    {
        sum += dataBuffer.at(i);
    }
    sum = (-sum & 0XFFFF);
    dataBuffer[6] = sum;
    command.SetSlave(process->SlaveID());
    command.SetDataBuffer(dataBuffer);
    command.SetAxis(8);
#ifndef Q_WS_X11
    if(process->ExecuteCommand(command).toBool())
#endif
    {
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Signal, armStruct_);
        host->SetAxisDefine(axisDefine_);
        host->SetPeripheryOutput(outDefine_);
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Fixture, dataBuffer.at(3));
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Resv1, dataBuffer.at(4));
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Resv2, dataBuffer.at(5));
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Xorsum, dataBuffer.at(6));
        host->SaveSystemConfig();
        QMessageBox::information(this, tr("Tips"), tr("Save Sucessfully!"));
        emit StructChanged();
        icMainFrame->UpdateAxisDefine_();
    }
    ICParametersSave::Instance()->SetSingleArm(ui->singleArmButton->isChecked());
    qDebug()<<"Struct = "<<armStruct_;
    qDebug()<<"Arm Define"<<axisDefine_;
}

void ICStructDefineFrame::on_doubleArmButton_toggled(bool checked)
{
    if(checked)
    {
        ui->subArmBox->setEnabled(true);
    }
    else
    {
        ui->subArmBox->setEnabled(false);
    }
//    checked ? armStruct_ &= 0xFEFF : armStruct_ |= 0x100;
}

void ICStructDefineFrame::on_mainArmDownLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetMainArmDownLimit(checked);
    checked ? armStruct_ |= 0x000C : armStruct_ &= 0xFFF7;
}

void ICStructDefineFrame::on_mainArmBackwardLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetMainArmBackwardLimit(checked);
    checked ? armStruct_ |= 0x0001 : armStruct_ &= 0xFFFE;
}

void ICStructDefineFrame::on_mainArmForwardLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetMainArmForwardLimit(checked);
    checked ? armStruct_ |= 0x0002 : armStruct_ &= 0xFFFD;
}

void ICStructDefineFrame::on_subArmDownLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetSubArmDownLimit(checked);
    checked ? armStruct_ |= 0x0300 : armStruct_ &= 0xFDFF;
}

void ICStructDefineFrame::on_subArmBackwardLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetSubArmBackwardLimit(checked);
    checked ? armStruct_ |= 0x0040 : armStruct_ &= 0xFFBF;
}

void ICStructDefineFrame::on_subArmForwardLimitButton_toggled(bool checked)
{
   checked ? armStruct_ |= 0x0080 : armStruct_ &= 0xFF7F;
}

void ICStructDefineFrame::OnAxisDefineChanged(int index)
{
    QComboBox* box = qobject_cast<QComboBox*>(sender());
    ICVirtualHost::GlobalVirtualHost()->CalAxisDefine(axisDefine_,
                                                      static_cast<ICVirtualHost::ICAxis>(boxToAxis_.value(box)),
                                                      static_cast<ICVirtualHost::ICAxisDefine>(indexToArmDefine_.value(index)));
}

void ICStructDefineFrame::OnOutputDefineChanged(int index)
{
    QComboBox* box = qobject_cast<QComboBox*>(sender());
    ICVirtualHost::GlobalVirtualHost()->CalPeripheryOutput(outDefine_,
                                                           outputDefineToNumber_.value(box),
                                                           index);
}


void ICStructDefineFrame::on_escapeComboBox_activated(int index)
{
    ICVirtualHost::GlobalVirtualHost()->SetEscapeWay(index);
}
