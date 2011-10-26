#include "icstructdefineframe.h"
#include "ui_icstructdefineframe.h"

#include "iccommandprocessor.h"
#include "iccommands.h"
#include "icvirtualhost.h"

#include <QVector>
#include <QMessageBox>
#include <QDebug>

ICStructDefineFrame::ICStructDefineFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICStructDefineFrame)
{
    ui->setupUi(this);
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    armStruct_ = host->SystemParameter(ICVirtualHost::SYS_ARM_CONFIG).toUInt();
    if(host->IsSingleArm())
    {
        ui->singleArmButton->setChecked(true);
        on_doubleArmButton_toggled(false);
    }
    else
    {
        ui->doubleArmButton->setChecked(true);
    }
    ui->mainArmForwardLimitButton->setChecked(host->HasMainArmForwardLimit());
    ui->mainArmBackwardLimitButton->setChecked(host->HasMainArmBackwardLimit());
    ui->mainArmDownLimitButton->setChecked(host->HasMainArmDownLimit());
    ui->subArmForwardLimitButton->setChecked(host->HasSubArmForwardLimit());
    ui->subArmBackwardLimitButton->setChecked(host->HasSubArmBackwardLimit());
    ui->subArmDownLimitButton->setChecked(host->HasSubArmDownLimit());
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
    QVector<uint> dataBuffer(8, 0);
    dataBuffer[0] = armStruct_;
    for(int i = 0; i != 6; ++i)
    {
        sum += dataBuffer.at(i);
    }
    sum = (-sum & 0XFFFF);
    dataBuffer[6] = sum;
    command.SetSlave(process->SlaveID());
    command.SetDataBuffer(dataBuffer);
    command.SetAxis(8);
    if(process->ExecuteCommand(command).toBool())
    {
        ICVirtualHost::GlobalVirtualHost()->SetSystemParameter(ICVirtualHost::SYS_ARM_CONFIG, armStruct_);
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        QMessageBox::information(this, tr("Tips"), tr("Save Sucessfully!"));
    }
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
    checked ? armStruct_ &= 0xFEFF : armStruct_ |= 0x100;
}

void ICStructDefineFrame::on_mainArmDownLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetMainArmDownLimit(checked);
    checked ? armStruct_ |= 0x0008 : armStruct_ &= 0xFFF7;
}

void ICStructDefineFrame::on_mainArmBackwardLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetMainArmBackwardLimit(checked);
     checked ? armStruct_ |= 0x0002 : armStruct_ &= 0xFFFD;
}

void ICStructDefineFrame::on_mainArmForwardLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetMainArmForwardLimit(checked);
    checked ? armStruct_ |= 0x0001 : armStruct_ &= 0xFFFE;
}

void ICStructDefineFrame::on_subArmDownLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetSubArmDownLimit(checked);
    checked ? armStruct_ |= 0x0080 : armStruct_ &= 0xFF7F;
}

void ICStructDefineFrame::on_subArmBackwardLimitButton_toggled(bool checked)
{
//    ICVirtualHost::GlobalVirtualHost()->SetSubArmBackwardLimit(checked);
    checked ? armStruct_ |= 0x0020 : armStruct_ &= 0xFFDF;
}

void ICStructDefineFrame::on_subArmForwardLimitButton_toggled(bool checked)
{
   checked ? armStruct_ |= 0x0010 : armStruct_ &= 0xFFEF;
}
