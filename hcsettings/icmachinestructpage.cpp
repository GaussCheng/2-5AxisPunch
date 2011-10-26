#include "icmachinestructpage.h"
#include "ui_icmachinestructpage.h"

#include <QButtonGroup>
#include <QMessageBox>

#include "icvirtualhost.h"
#include "icparameterssave.h"
#include "icstructdefineframe.h"
#include "ichctimeframe.h"

ICMachineStructPage::ICMachineStructPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMachineStructPage),
    structPage_(NULL),
    timePage_(NULL)
{
    ui->setupUi(this);
    InitInterface();
    buttonGroup_ = new QButtonGroup();

    ui->axisXToolButton->setText(tr("X1 Axis"));
    ui->axisYToolButton->setText(tr("Y1 Axis"));
    ui->axisZToolButton->setText(tr("Z Axis"));
    ui->axisPToolButton->setText(tr("X2 Axis"));
    ui->axisQToolButton->setText(tr("Y2 Axis"));
    ui->axisAToolButton->setText(tr("A Axis"));
    ui->axisBToolButton->setText(tr("B Axis"));
    ui->axisCToolButton->setText(tr("C Axis"));
    ui->axisDefineToolButton->setText(tr("Axis Define"));
    ui->structDefButton->setText(tr("Struct Define"));
    ui->timeLimitButton->setText(tr("Time"));
    buttonGroup_->addButton(ui->axisXToolButton);
    buttonGroup_->addButton(ui->axisYToolButton);
    buttonGroup_->addButton(ui->axisZToolButton);
    buttonGroup_->addButton(ui->axisPToolButton);
    buttonGroup_->addButton(ui->axisQToolButton);
    buttonGroup_->addButton(ui->axisAToolButton);
    buttonGroup_->addButton(ui->axisBToolButton);
    buttonGroup_->addButton(ui->axisCToolButton);
    buttonGroup_->addButton(ui->axisDefineToolButton);
    buttonGroup_->addButton(ui->structDefButton);
    buttonGroup_->addButton(ui->timeLimitButton);
    buttonGroup_->setExclusive(true);
    QAbstractButton* button;
    foreach(button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }

    ui->axisXToolButton->click();
    /*axis define*/
    boxToAxis_.insert(ui->x1Box, ICVirtualHost::ICAxis_AxisX1);
    boxToAxis_.insert(ui->y1Box, ICVirtualHost::ICAxis_AxisY1);
    boxToAxis_.insert(ui->x2Box, ICVirtualHost::ICAxis_AxisX2);
    boxToAxis_.insert(ui->y2Box, ICVirtualHost::ICAxis_AxisY2);
    boxToAxis_.insert(ui->zBox, ICVirtualHost::ICAxis_AxisZ);
    boxToAxis_.insert(ui->aBox, ICVirtualHost::ICAxis_AxisA);
    boxToAxis_.insert(ui->bBox, ICVirtualHost::ICAxis_AxisB);
    boxToAxis_.insert(ui->cBox, ICVirtualHost::ICAxis_AxisC);
    defineToIndex_.insert(ICVirtualHost::ICAxisDefine_None, 0);
    defineToIndex_.insert(ICVirtualHost::ICAxisDefine_Pneumatic, 1);
    defineToIndex_.insert(ICVirtualHost::ICAxisDefine_Servo, 2);
    indexToDefine_.insert(0, ICVirtualHost::ICAxisDefine_None);
    indexToDefine_.insert(1, ICVirtualHost::ICAxisDefine_Pneumatic);
    indexToDefine_.insert(2, ICVirtualHost::ICAxisDefine_Servo);
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    QList<QComboBox*> boxs = findChildren<QComboBox*>();
    axisDefine_ = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
    for(int i = 0; i != boxs.size(); ++i)
    {
        boxs[i]->setCurrentIndex(defineToIndex_.value(host->AxisDefine(static_cast<ICVirtualHost::ICAxis>(boxToAxis_.value(boxs.at(i))))));
        connect(boxs[i],
                SIGNAL(currentIndexChanged(int)),
                SLOT(OnAxisDefineChanged(int)));
    }
//    ui->x1Box->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisX11)));
//    ui->y1Box->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisY11)));
//    ui->zBox->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisZ)));
//    ui->x2Box->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisX12)));
//    ui->y2Box->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisY12)));
//    ui->aBox->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisA)));
//    ui->bBox->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisB)));
//    ui->cBox->setCurrentIndex(defineToIndex_.value(host->AxisDefine(ICVirtualHost::ICAxis_ICVirtualHost::ICAxis_AxisC)));
}

ICMachineStructPage::~ICMachineStructPage()
{
    delete buttonGroup_;
    delete ui;
}

void ICMachineStructPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ui->axisXToolButton->setText(tr("X1 Axis"));
        ui->axisYToolButton->setText(tr("Y1 Axis"));
        ui->axisZToolButton->setText(tr("Z Axis"));
        ui->axisPToolButton->setText(tr("X2 Axis"));
        ui->axisQToolButton->setText(tr("Y2 Axis"));
        ui->axisAToolButton->setText(tr("A Axis"));
        ui->axisBToolButton->setText(tr("B Axis"));
        ui->axisCToolButton->setText(tr("C Axis"));
          ui->axisDefineToolButton->setText(tr("Axis Define"));
        ui->structDefButton->setText(tr("Struct Define"));
        ui->timeLimitButton->setText(tr("Time"));
        break;
    default:
        break;
    }
}

void ICMachineStructPage::hideEvent(QHideEvent *e)
{
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    QWidget::hideEvent(e);
}

void ICMachineStructPage::on_axisXToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisX1);
}

void ICMachineStructPage::on_axisYToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisY1);
}

void ICMachineStructPage::on_axisZToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisZ);
}

void ICMachineStructPage::on_axisPToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisX2);
}

void ICMachineStructPage::on_axisQToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisY2);
}

void ICMachineStructPage::on_axisAToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisA);
}

void ICMachineStructPage::on_axisBToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisB);
}

void ICMachineStructPage::on_axisCToolButton_clicked()
{
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisC);
}

void ICMachineStructPage::on_axisDefineToolButton_clicked()
{
    ui->content->setCurrentIndex(1);
}

void ICMachineStructPage::on_structDefButton_clicked()
{
    if(structPage_ == NULL)
    {
        structPage_ = new ICStructDefineFrame();
        ui->content->addWidget(structPage_);
    }
    ui->content->setCurrentWidget(structPage_);
}

void ICMachineStructPage::on_timeLimitButton_clicked()
{
    if(timePage_ == NULL)
    {
        timePage_ = new ICHCTimeFrame();
        ui->content->addWidget(timePage_);
    }
    ui->content->setCurrentWidget(timePage_);
}

void ICMachineStructPage::SetCurrentAxis(int axis)
{
    ui->content->setCurrentIndex(0);
    currentAxis_ = axis;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ICVirtualHost::ICSystemParameter machineLangth;
    ICVirtualHost::ICSystemParameter maxLangth;
    ICVirtualHost::ICSystemParameter iSafe;
    ICVirtualHost::ICSystemParameter oSafe;
    QString minText;
    QString maxText;

    double total = 0;
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        machineLangth = ICVirtualHost::SYS_X_Length;
        maxLangth = ICVirtualHost::SYS_X_Maxium;
        iSafe = ICVirtualHost::SYS_X_InSafe;
        oSafe = ICVirtualHost::SYS_X_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("X");
        minText = tr("Min pos inside mold");
        maxText = tr("Max pos inside mold");
        ui->distanceRotationEdit->SetDecimalPlaces(2);
        rotateValidator_->setTop(32767);

    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        machineLangth = ICVirtualHost::SYS_Y_Length;
        maxLangth = ICVirtualHost::SYS_Y_Maxium;
        iSafe = ICVirtualHost::SYS_Y_InSafe;
        oSafe = ICVirtualHost::SYS_Y_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Y");
        minText = tr("Max standby pos");
        maxText = tr("Leave origin pos");
        ui->distanceRotationEdit->SetDecimalPlaces(2);
        rotateValidator_->setTop(32767);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        machineLangth = ICVirtualHost::SYS_Z_Length;
        maxLangth = ICVirtualHost::SYS_Z_Maxium;
        iSafe = ICVirtualHost::SYS_Z_InSafe;
        oSafe = ICVirtualHost::SYS_Z_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Z");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
        ui->distanceRotationEdit->SetDecimalPlaces(2);
        rotateValidator_->setTop(32767);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        machineLangth = ICVirtualHost::SYS_P_Length;
        maxLangth = ICVirtualHost::SYS_P_Maxium;
        iSafe = ICVirtualHost::SYS_P_InSafe;
        oSafe = ICVirtualHost::SYS_P_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("P");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
        ui->distanceRotationEdit->SetDecimalPlaces(2);
        rotateValidator_->setTop(32767);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        machineLangth = ICVirtualHost::SYS_Q_Length;
        maxLangth = ICVirtualHost::SYS_Q_Maxium;
        iSafe = ICVirtualHost::SYS_Q_InSafe;
        oSafe = ICVirtualHost::SYS_Q_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Q");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
        ui->distanceRotationEdit->SetDecimalPlaces(2);
        rotateValidator_->setTop(32767);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        machineLangth = ICVirtualHost::SYS_A_Length;
        maxLangth = ICVirtualHost::SYS_A_Maxium;
        iSafe = ICVirtualHost::SYS_A_InSafe;
        oSafe = ICVirtualHost::SYS_A_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("A");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
        ui->distanceRotationEdit->SetDecimalPlaces(1);
        rotateValidator_->setTop(3600);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        machineLangth = ICVirtualHost::SYS_B_Length;
        maxLangth = ICVirtualHost::SYS_B_Maxium;
        iSafe = ICVirtualHost::SYS_B_InSafe;
        oSafe = ICVirtualHost::SYS_B_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("B");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
        ui->distanceRotationEdit->SetDecimalPlaces(1);
        rotateValidator_->setTop(3600);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        machineLangth = ICVirtualHost::SYS_C_Length;
        maxLangth = ICVirtualHost::SYS_C_Maxium;
        iSafe = ICVirtualHost::SYS_C_InSafe;
        oSafe = ICVirtualHost::SYS_C_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("C");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
        ui->distanceRotationEdit->SetDecimalPlaces(1);
        rotateValidator_->setTop(3600);
    }
    else
    {
        return;
    }
    ui->mechanicalLengthLineEdit->SetThisIntToThisText(host->SystemParameter(machineLangth).toInt());
    ui->maximumDisplacementLineEdit->SetThisIntToThisText(host->SystemParameter(maxLangth).toInt());
    ui->internalSecurityZoneLineEdit->SetThisIntToThisText(host->SystemParameter(iSafe).toInt());
    ui->externalSecurityZoneLineEdit->SetThisIntToThisText(host->SystemParameter(oSafe).toInt());
    QString format = QString("%.%1f").arg(ui->distanceRotationEdit->DecimalPlaces());
    ui->distanceRotationEdit->setText(QString().sprintf(format.toAscii(), total));
//    ui->distanceRotationEdit->SetThisIntToThisText(total);
    ui->minLabel->setText(minText);
    ui->maxLabel->setText(maxText);
//    maxMoveValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
}

QList<uint> ICMachineStructPage::GetCurrentStatus_() const
{
    QList<uint> ret;
    double machineLength = ui->mechanicalLengthLineEdit->text().toDouble();
    ret.append(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    ret.append(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());
    ret.append(ui->internalSecurityZoneLineEdit->TransThisTextToThisInt());
    ret.append(ui->externalSecurityZoneLineEdit->TransThisTextToThisInt());
    uint totalH;
    uint totalL;
    uint total = machineLength * 10000 / ui->distanceRotationEdit->text().toDouble();
    totalH = (total >> 16) & 0XFFFF;
    totalL = total & 0XFFFF;
    ret.append(totalL);
    ret.append(totalH);
    int sum = 0;
    for(int i = 0; i != ret.size(); ++i)
    {
        sum += ret.at(i);
    }
    sum = (-sum & 0xFFFF);
    ret.append(sum);
    return ret;
    //    ret.append();
}

bool ICMachineStructPage::SetCurrentStatus_(const QList<uint> &status)
{
    Q_ASSERT_X(status.size() == 7, "ICMachineStructPage::SetCurrentStatus", "status'size is less than 7!");
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    ICVirtualHost::ICSystemParameter machineLangth;
    ICVirtualHost::ICSystemParameter maxLangth;
    ICVirtualHost::ICSystemParameter iSafe;
    ICVirtualHost::ICSystemParameter oSafe;
    ICVirtualHost::ICSystemParameter tH;
    ICVirtualHost::ICSystemParameter tL;
    ICVirtualHost::ICSystemParameter sum;
    QString axisName;
    int axis;
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        machineLangth = ICVirtualHost::SYS_X_Length;
        maxLangth = ICVirtualHost::SYS_X_Maxium;
        iSafe = ICVirtualHost::SYS_X_InSafe;
        oSafe = ICVirtualHost::SYS_X_OutSafe;
        tH = ICVirtualHost::SYS_X_TotalH;
        tL = ICVirtualHost::SYS_X_TotalL;
        sum = ICVirtualHost::SYS_X_XorSum;
        axisName = "X";
        axis = 0;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        machineLangth = ICVirtualHost::SYS_Y_Length;
        maxLangth = ICVirtualHost::SYS_Y_Maxium;
        iSafe = ICVirtualHost::SYS_Y_InSafe;
        oSafe = ICVirtualHost::SYS_Y_OutSafe;
        tH = ICVirtualHost::SYS_Y_TotalH;
        tL = ICVirtualHost::SYS_Y_TotalL;
        sum = ICVirtualHost::SYS_Y_XorSum;
        axisName = "Y";
        axis = 1;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        machineLangth = ICVirtualHost::SYS_Z_Length;
        maxLangth = ICVirtualHost::SYS_Z_Maxium;
        iSafe = ICVirtualHost::SYS_Z_InSafe;
        oSafe = ICVirtualHost::SYS_Z_OutSafe;
        tH = ICVirtualHost::SYS_Z_TotalH;
        tL = ICVirtualHost::SYS_Z_TotalL;
        sum = ICVirtualHost::SYS_Z_XorSum;
        axisName = "Z";
        axis = 2;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        machineLangth = ICVirtualHost::SYS_P_Length;
        maxLangth = ICVirtualHost::SYS_P_Maxium;
        iSafe = ICVirtualHost::SYS_P_InSafe;
        oSafe = ICVirtualHost::SYS_P_OutSafe;
        tH = ICVirtualHost::SYS_P_TotalH;
        tL = ICVirtualHost::SYS_P_TotalL;
        sum = ICVirtualHost::SYS_P_XorSum;
        axisName = "P";
        axis = 3;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        machineLangth = ICVirtualHost::SYS_Q_Length;
        maxLangth = ICVirtualHost::SYS_Q_Maxium;
        iSafe = ICVirtualHost::SYS_Q_InSafe;
        oSafe = ICVirtualHost::SYS_Q_OutSafe;
        tH = ICVirtualHost::SYS_Q_TotalH;
        tL = ICVirtualHost::SYS_Q_TotalL;
        sum = ICVirtualHost::SYS_Q_XorSum;
        axisName = "Q";
        axis = 4;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        machineLangth = ICVirtualHost::SYS_A_Length;
        maxLangth = ICVirtualHost::SYS_A_Maxium;
        iSafe = ICVirtualHost::SYS_A_InSafe;
        oSafe = ICVirtualHost::SYS_A_OutSafe;
        tH = ICVirtualHost::SYS_A_TotalH;
        tL = ICVirtualHost::SYS_A_TotalL;
        sum = ICVirtualHost::SYS_A_XorSum;
        axisName = "A";
        axis = 5;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        machineLangth = ICVirtualHost::SYS_B_Length;
        maxLangth = ICVirtualHost::SYS_B_Maxium;
        iSafe = ICVirtualHost::SYS_B_InSafe;
        oSafe = ICVirtualHost::SYS_B_OutSafe;
        tH = ICVirtualHost::SYS_B_TotalH;
        tL = ICVirtualHost::SYS_B_TotalL;
        sum = ICVirtualHost::SYS_B_XorSum;
        axisName = "B";
        axis = 6;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        machineLangth = ICVirtualHost::SYS_C_Length;
        maxLangth = ICVirtualHost::SYS_C_Maxium;
        iSafe = ICVirtualHost::SYS_C_InSafe;
        oSafe = ICVirtualHost::SYS_C_OutSafe;
        tH = ICVirtualHost::SYS_C_TotalH;
        tL = ICVirtualHost::SYS_C_TotalL;
        sum = ICVirtualHost::SYS_C_XorSum;
        axisName = "C";
        axis = 7;
    }
    else
    {
        return false;
    }
    ICSetAxisConfigsCommand command;
    ICCommandProcessor* processor = ICCommandProcessor::Instance();
    command.SetSlave(processor->SlaveID());
    command.SetAxis(axis);
    command.SetDataBuffer(status.toVector());
    if(processor->ExecuteCommand(command).toBool())
    {
        host->SetSystemParameter(machineLangth, status.at(0));
        host->SetSystemParameter(maxLangth, status.at(1));
        host->SetSystemParameter(iSafe, status.at(2));
        host->SetSystemParameter(oSafe, status.at(3));
        host->SetSystemParameter(tL, status.at(4));
        host->SetSystemParameter(tH, status.at(5));
        host->SetSystemParameter(sum, status.at(6));
        ICParametersSave::Instance()->SetDistanceRotation(axisName, ui->distanceRotationEdit->text().toDouble());
        qDebug()<<ICParametersSave::Instance()->DistanceRotation(axisName);
        return true;
    }
    return false;
}

void ICMachineStructPage::on_saveToolButton_clicked()
{
    QList<uint> status;
    status = GetCurrentStatus_();
    if(SetCurrentStatus_(status))
    {
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        host->SaveAxisParam(currentAxis_);
//        host->ReConfigure();
        QMessageBox::information(this, tr("Information"), tr("Save Successfully!"));
    }
}

void ICMachineStructPage::InitInterface()
{
    QIntValidator * intValidator = new QIntValidator(0, 32767, this);
    ui->mechanicalLengthLineEdit->SetDecimalPlaces(1);
    ui->mechanicalLengthLineEdit->setValidator(intValidator);
    ui->maximumDisplacementLineEdit->SetDecimalPlaces(1);
    maxMoveValidator_ = new QIntValidator(0, 32767, this);
    ui->maximumDisplacementLineEdit->setValidator(maxMoveValidator_);
    ui->internalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->internalSecurityZoneLineEdit->setValidator(intValidator);
    ui->externalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->externalSecurityZoneLineEdit->setValidator(intValidator);
    ui->distanceRotationEdit->SetDecimalPlaces(2);
    rotateValidator_ = new QIntValidator(0, 32767, this);
    ui->distanceRotationEdit->setValidator(rotateValidator_);
}

void ICMachineStructPage::OnAxisDefineChanged(int index)
{
    QComboBox* box = qobject_cast<QComboBox*>(sender());
    ICVirtualHost::GlobalVirtualHost()->CalAxisDefine(axisDefine_,
                                                      static_cast<ICVirtualHost::ICAxis>(boxToAxis_.value(box)),
                                                      static_cast<ICVirtualHost::ICAxisDefine>(indexToDefine_.value(index)));
    qDebug()<<axisDefine_;
}

void ICMachineStructPage::on_saveButton_clicked()
{
    ICVirtualHost::GlobalVirtualHost()->SetAxisDefine(axisDefine_);
    QMessageBox::information(this, tr("Tips"), tr("Save Sucessfully!"));
}
