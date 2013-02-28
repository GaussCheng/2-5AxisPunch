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
    buttongroup_ = new QButtonGroup ;

    ui->buttonGroupA->setId(ui->normalABox,0);
    ui->buttonGroupA->setId(ui->extentABox,1);
    ui->buttonGroupB->setId(ui->normalBBox,0);
    ui->buttonGroupB->setId(ui->extentBBox,1);
    ui->buttonGroupC->setId(ui->normalCBox,0);
    ui->buttonGroupC->setId(ui->extentCBox,1);
    ui->buttonGroupD->setId(ui->normalDBox,0);
    ui->buttonGroupD->setId(ui->extentDBox,1);
    ui->buttonGroupE->setId(ui->normalEBox,0);
    ui->buttonGroupE->setId(ui->extentEBox,1);
    ui->buttonGroupF->setId(ui->normalFBox,0);
    ui->buttonGroupF->setId(ui->extentFBox,1);
    ui->buttonGroupG->setId(ui->normalGBox,0);
    ui->buttonGroupG->setId(ui->extentGBox,1);
    ui->buttonGroupH->setId(ui->normalHBox,0);
    ui->buttonGroupH->setId(ui->extentHBox,1);
    if(ICParametersSave::Instance()->IsSingleArm())
    {
        ui->singleArmButton->setChecked(true);
        on_doubleArmButton_toggled(false);
    }
    else
    {
        ui->doubleArmButton->click();
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
    InitEscapeBox() ;
    buttongrouplist.append(ui->buttonGroupA);
    buttongrouplist.append(ui->buttonGroupB);
    buttongrouplist.append(ui->buttonGroupC);
    buttongrouplist.append(ui->buttonGroupD);
    buttongrouplist.append(ui->buttonGroupE);
    buttongrouplist.append(ui->buttonGroupF);
    buttongrouplist.append(ui->buttonGroupG);
    buttongrouplist.append(ui->buttonGroupH);

    buttonslist_.append(ui->buttonGroupA->buttons());
    buttonslist_.append(ui->buttonGroupB->buttons());
    buttonslist_.append(ui->buttonGroupC->buttons());
    buttonslist_.append(ui->buttonGroupD->buttons());
    buttonslist_.append(ui->buttonGroupE->buttons());
    buttonslist_.append(ui->buttonGroupF->buttons());
    buttonslist_.append(ui->buttonGroupG->buttons());
    buttonslist_.append(ui->buttonGroupH->buttons());

    for(int i = 0 ; i < buttongrouplist.size() ; ++i)
        outputDefineToNumber_.insert(buttongrouplist[i],i) ;


//    outputDefineToNumber_.insert(ui->outABox, 0);
//    outputDefineToNumber_.insert(ui->outBBox, 1);
//    outputDefineToNumber_.insert(ui->outCBox, 2);
//    outputDefineToNumber_.insert(ui->outDBox, 3);
//    outputDefineToNumber_.insert(ui->outEBox, 4);
//    outputDefineToNumber_.insert(ui->outFBox, 5);
//    outputDefineToNumber_.insert(ui->outGBox, 6);
//    outputDefineToNumber_.insert(ui->outHBox, 7);

//    numberToOutputDefine_.insert(0, ui->outABox);
//    numberToOutputDefine_.insert(1, ui->outBBox);
//    numberToOutputDefine_.insert(2, ui->outCBox);
//    numberToOutputDefine_.insert(3, ui->outDBox);
//    numberToOutputDefine_.insert(4, ui->outEBox);
//    numberToOutputDefine_.insert(5, ui->outFBox);
//    numberToOutputDefine_.insert(6, ui->outGBox);
//    numberToOutputDefine_.insert(7, ui->outHBox);
    outDefine_ = host->SystemParameter(ICVirtualHost::ICVirtualHost::SYS_Config_Out).toInt();
//    boxs = ui->outDefineBox->findChildren<QComboBox*>();
 //   boxs = ui->outDefineBox->findChildren<QButtonGroup*>();
    for(int i = 0; i != buttongrouplist.size(); ++i)
    {
//        boxs[i]->setCurrentIndex(host->PeripheryOutput(outputDefineToNumber_.value(boxs.at(i))));
        if(host->PeripheryOutput(outputDefineToNumber_.value(buttongrouplist.at(i))) == 0)
        {
            buttongrouplist.at(i)->button(0)->setChecked(true);
//            buttonslist_[i][0]->setChecked(true);//不能保证从容器中返回的成员是按顺序的，所以不能这样写，将会导致有些数据会变化
        }
        else if(host->PeripheryOutput(outputDefineToNumber_.value(buttongrouplist.at(i))) == 1)
        {
            buttongrouplist.at(i)->button(1)->setChecked(true);
//            buttonslist_[i][1]->setChecked(true);
        }

        buttongrouplist[i]->setExclusive(true);
        connect(buttongrouplist[i],
                SIGNAL(buttonClicked(int)),
                SLOT(OnOutputDefineChanged(int)));
    }

//    ui->fixtureSelectBox->setCurrentIndex(host->FixtureDefine());
  //  ui->escapeComboBox->setCurrentIndex(host->EscapeWay());
    if(host->EscapeWay() == 0)
        ui->useCheckBox->setChecked(true);
    if(host->EscapeWay() == 1)
        ui->noUseCheckBox->setChecked(true);
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
     //  ui->retranslateUi(this);
       retranslateUi_();
        break;
    default:
        break;
    }
}

void ICStructDefineFrame::retranslateUi_()
{
    this->setWindowTitle(tr("Form"));
    ui->signalDefineBox->setTitle(tr("Signal Define"));
    ui->singleArmButton->setText(tr("Single Arm"));
    ui->doubleArmButton->setText(tr("Double Arm"));
    ui->mainArmBox->setTitle(tr("Main Arm"));
    ui->mainArmDownLimitButton->setText(tr("Down Limit"));
    ui->mainArmBackwardLimitButton->setText(tr("Backword Limit"));
    ui->mainArmForwardLimitButton->setText(tr("Forward Limit"));
    ui->subArmBox->setTitle(tr("Sub Arm"));
    ui->subArmDownLimitButton->setText(tr("Down Limit"));
    ui->subArmBackwardLimitButton->setText(tr("Backword Limit"));
    ui->subArmForwardLimitButton->setText(tr("Forward Limit"));
    ui->armDefineBox->setTitle(tr("Arm Define"));
    ui->label_5->setText(tr("X1"));
    ui->x1Box->setItemText(0,tr("None"));
    ui->x1Box->setItemText(1,tr("Pneumatic"));
    ui->x1Box->setItemText(2,tr("Servo"));
    ui->label_14->setText(tr("C"));
    ui->y1Box->setItemText(0,tr("None"));
    ui->y1Box->setItemText(1,tr("Pneumatic"));
    ui->y1Box->setItemText(2,tr("Servo"));
    ui->label_13->setText(tr("Z"));
    ui->cBox->setItemText(0,tr("None"));
    ui->cBox->setItemText(1,tr("Pneumatic"));
    ui->cBox->setItemText(2,tr("Servo"));
    ui->label_15->setText(tr("A"));
    ui->aBox->setItemText(0,tr("None"));
    ui->aBox->setItemText(1,tr("Pneumatic"));
    ui->aBox->setItemText(2,tr("Servo"));
    ui->label_16->setText(tr("B"));
    ui->zBox->setItemText(0,tr("None"));
    ui->zBox->setItemText(1,tr("Pneumatic"));
    ui->zBox->setItemText(2,tr("Servo"));
    ui->label_7->setText(tr("Y1"));
    ui->bBox->setItemText(0,tr("None"));
    ui->bBox->setItemText(1,tr("Pneumatic"));
    ui->bBox->setItemText(2,tr("Servo"));
    ui->label_6->setText(tr("X2"));
    ui->x2Box->setItemText(0,tr("None"));
    ui->x2Box->setItemText(1,tr("Pneumatic"));
    ui->x2Box->setItemText(2,tr("Servo"));
    ui->label_12->setText(tr("Y2"));
    ui->y2Box->setItemText(0,tr("None"));
    ui->y2Box->setItemText(1,tr("Pneumatic"));
    ui->y2Box->setItemText(2,tr("Servo"));
    ui->outDefineBox->setTitle(tr("Out Define"));
    ui->label->setText(tr("1"));
//    ui->outABox->setItemText(0,tr("Normal"));
//    ui->outABox->setItemText(1,tr("Extent"));
//    ui->label_8->setText(tr("5"));
//    ui->outEBox->setItemText(0,tr("Normal"));
//    ui->outEBox->setItemText(1,tr("Extent"));
//    ui->label_2->setText(tr("2"));
//    ui->outBBox->setItemText(0,tr("Normal"));
//    ui->outBBox->setItemText(1,tr("Extent"));
//    ui->label_9->setText(tr("6"));
//    ui->outFBox->setItemText(0,tr("Normal"));
//    ui->outFBox->setItemText(1,tr("Extent"));
//    ui->label_3->setText(tr("3"));
//    ui->outCBox->setItemText(0,tr("Normal"));
//    ui->outCBox->setItemText(1,tr("Extent"));
//    ui->label_11->setText(tr("7"));
//    ui->outGBox->setItemText(0,tr("Normal"));
//    ui->outGBox->setItemText(1,tr("Extent"));
//    ui->label_4->setText(tr("4"));
//    ui->outDBox->setItemText(0,tr("Normal"));
//    ui->outDBox->setItemText(1,tr("Extent"));
//    ui->label_10->setText(tr("8"));
//    ui->outHBox->setItemText(0,tr("Normal"));
//    ui->outHBox->setItemText(1,tr("Extent"));
    ui->normalABox->setText(tr("Normal"));
    ui->extentABox->setText(tr("Extent"));
    ui->normalBBox->setText(tr("Normal"));
    ui->extentBBox->setText(tr("Extent"));
    ui->normalCBox->setText(tr("Normal"));
    ui->extentCBox->setText(tr("Extent"));
    ui->normalDBox->setText(tr("Normal"));
    ui->extentDBox->setText(tr("Extent"));
    ui->normalEBox->setText(tr("Normal"));
    ui->extentEBox->setText(tr("Extent"));
    ui->normalFBox->setText(tr("Normal"));
    ui->extentFBox->setText(tr("Extent"));
    ui->normalGBox->setText(tr("Normal"));
    ui->extentGBox->setText(tr("Extent"));
    ui->normalHBox->setText(tr("Normal"));
    ui->extentHBox->setText(tr("Extent"));
    ui->fixtureDefineBox_2->setTitle(tr("Other Define"));
    ui->label_18->setText(tr("Escape"));
//    ui->escapeComboBox->setItemText(0,tr("Use"));
//    ui->escapeComboBox->setItemText(1,tr("No Use"));
    ui->useCheckBox->setText(tr("Use"));
    ui->noUseCheckBox->setText(tr("No Use"));
    ui->saveButton->setText(tr("Save"));
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
//    dataBuffer[3] = ICVirtualHost::GlobalVirtualHost()->FixtureDefineSwitch(ui->fixtureSelectBox->currentIndex());
    dataBuffer[3] = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Fixture).toUInt();
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
//        host->SystemParameter(ICVirtualHost::SYS_Function);
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
    QButtonGroup* box = qobject_cast<QButtonGroup*>(sender());
    ICVirtualHost::GlobalVirtualHost()->CalPeripheryOutput(outDefine_,
                                                           outputDefineToNumber_.value(box),
                                                           index);
}


void ICStructDefineFrame::escapeBoxChange()
{
    ICVirtualHost::GlobalVirtualHost()->SetEscapeWay(buttongroup_->checkedId());
}

void ICStructDefineFrame::InitEscapeBox()
{
    buttongroup_->addButton(ui->useCheckBox,0);
    buttongroup_->addButton(ui->noUseCheckBox,1);
    QList<QAbstractButton*> buttons = buttongroup_->buttons();
    for(int i = 0; i != buttons.size(); ++i)
    {
        buttons[i]->setCheckable(true);
        connect(buttons.at(i),
                SIGNAL(clicked()),
                this,
                SLOT(escapeBoxChange()));
    }
    buttongroup_->setExclusive(true);
}
