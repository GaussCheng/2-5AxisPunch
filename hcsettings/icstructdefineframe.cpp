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
    armTypeMaskMap_.insert("x1", 0);
    armTypeMaskMap_.insert("y1", 2);
    armTypeMaskMap_.insert("x2", 4);
    armTypeMaskMap_.insert("y2", 6);
    armTypeMaskMap_.insert("arm", 8);
    connect(ui->x1Select,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(OnSelectChanged(int)));
    connect(ui->x2Select,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(OnSelectChanged(int)));
    connect(ui->y1Select,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(OnSelectChanged(int)));
    connect(ui->y2Select,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(OnSelectChanged(int)));
    connect(ui->armSelect,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(OnSelectChanged(int)));
    armStruct_ = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_ARM_CONFIG).toUInt();
    ui->x1Select->setCurrentIndex(armStruct_ & 0x0003);
    ui->y1Select->setCurrentIndex(((armStruct_ & 0x000C) >> 2) == 1 ? 0 : 1);
    ui->x2Select->setCurrentIndex((armStruct_ & 0x0030) >> 4);
    ui->y2Select->setCurrentIndex(((armStruct_ & 0x00C0) >> 6) == 1 ? 0 : 1);
    ui->armSelect->setCurrentIndex(((armStruct_ & 0x0300) >> 8) == 1 ? 0 : 1);
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

void ICStructDefineFrame::OnSelectChanged(int index)
{
    QString armType = sender()->property("armType").toString();
    uint value = 0;
    if(armType == "y1" || armType == "y2")
    {
        value = (index == 0) ? 1 : 3;
    }
    else
    {
        value = index;
    }
    uint mask = ~(3 << (armTypeMaskMap_.value(armType)));
    qDebug()<<mask;
    armStruct_ &= mask;
    armStruct_ |= (value << (armTypeMaskMap_.value(armType)));
    qDebug()<<armStruct_;
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
    command.SetAxis(3);
    if(process->ExecuteCommand(command).toBool())
    {
        ICVirtualHost::GlobalVirtualHost()->SetSystemParameter(ICVirtualHost::SYS_ARM_CONFIG, armStruct_);
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        QMessageBox::information(this, tr("Tips"), tr("Save Sucessfully!"));
    }
}
