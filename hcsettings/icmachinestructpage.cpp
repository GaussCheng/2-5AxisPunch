#include "icmachinestructpage.h"
#include "ui_icmachinestructpage.h"

#include <QButtonGroup>
#include "icmessagebox.h"

#include "icvirtualhost.h"
#include "icparameterssave.h"
#include "icstructdefineframe.h"
#include "ichctimeframe.h"
#include "icactioncommand.h"
#include "icmodifyframe.h"
#include "icmachineconfigpage.h"

ICMachineStructPage::ICMachineStructPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMachineStructPage),
    structPage_(NULL),
    timePage_(NULL),
    servoPage_(NULL),
    axisDefine_(-1)
{
    ui->setupUi(this);
    InitInterface();
    ui->servoToolButton->hide();

    buttonGroup_ = new QButtonGroup();

    ui->axisXToolButton->setText(tr("X Axis"));
    ui->axisYToolButton->setText(tr("Y Axis"));
    ui->axisZToolButton->setText(tr("S Axis"));
    ui->axisPToolButton->setText(tr("R Axis"));
    ui->axisQToolButton->setText(tr("T Axis"));
    ui->axisAToolButton->setText(tr("A Axis"));
    ui->axisBToolButton->setText(tr("B Axis"));
    ui->axisCToolButton->setText(tr("C Axis"));
    ui->structDefButton->setText(tr("Struct Define"));
    ui->timeLimitButton->setText(tr("Time"));
    ui->servoToolButton->setText(tr("Servo"));

    buttonGroup_->addButton(ui->axisXToolButton);
    buttonGroup_->addButton(ui->axisYToolButton);
    buttonGroup_->addButton(ui->axisZToolButton);
    buttonGroup_->addButton(ui->axisPToolButton);
    buttonGroup_->addButton(ui->axisQToolButton);
    buttonGroup_->addButton(ui->axisAToolButton);
    buttonGroup_->addButton(ui->axisBToolButton);
    buttonGroup_->addButton(ui->axisCToolButton);
    buttonGroup_->addButton(ui->structDefButton);
    buttonGroup_->addButton(ui->timeLimitButton);
    buttonGroup_->addButton(ui->servoToolButton);
    buttonGroup_->setExclusive(true);
    QAbstractButton* button;
    foreach(button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }

    ui->axisXToolButton->click();

    connect(&refreshTimer_,
            SIGNAL(timeout()),
            SLOT(StatusRefresh()));

    editorToConfigIDs_.insert(ui->mechanicalLengthLineEdit, ICConfigString::kCS_AXIS_Length_X1);
    editorToConfigIDs_.insert(ui->maximumDisplacementLineEdit, ICConfigString::kCS_AXIS_Move_Limit_X1);
    editorToConfigIDs_.insert(ui->internalSecurityZoneLineEdit, ICConfigString::kCS_AXIS_Min_X1);
    editorToConfigIDs_.insert(ui->externalSecurityZoneLineEdit, ICConfigString::kCS_AXIS_Max_X1);
    editorToConfigIDs_.insert(ui->distanceRotationEdit, ICConfigString::kCS_AXIS_Rotate_X1);

    ICLogInit

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
        ui->axisXToolButton->setText(tr("X Axis"));
        ui->axisYToolButton->setText(tr("Y Axis"));
        ui->axisZToolButton->setText(tr("S Axis"));
        ui->axisPToolButton->setText(tr("R Axis"));
        ui->axisQToolButton->setText(tr("T Axis"));
        ui->axisAToolButton->setText(tr("A Axis"));
        ui->axisBToolButton->setText(tr("B Axis"));
        ui->axisCToolButton->setText(tr("C Axis"));
        ui->structDefButton->setText(tr("Struct Define"));
        ui->timeLimitButton->setText(tr("Time"));
        ui->servoToolButton->setText(tr("Servo"));

        break;
    default:
        break;
    }
}

void ICMachineStructPage::hideEvent(QHideEvent *e)
{
//    int machineCount = 0;
//    for(int i = 0; i != punchButtons_.size(); ++i)
//    {
//        if(punchButtons_.at(i)->isChecked())
//        {
//            machineCount |= (1 << i);
//        }
//    }
//    ICVirtualHost::GlobalVirtualHost()->SetSystemParameter(ICVirtualHost::SYS_Config_Fixture, machineCount);
    if(ICVirtualHost::GlobalVirtualHost()->IsParamChanged())
    {
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    refreshTimer_.stop();
//    disconnect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(StatusRefreshed()),
//            this,
//            SLOT(StatusRefresh()));
    QWidget::hideEvent(e);
}

void ICMachineStructPage::showEvent(QShowEvent *e)
{
    UpdateAxisDefine_();
//    connect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(StatusRefreshed()),
//            this,
//            SLOT(StatusRefresh()));
    if(!ui->axisXToolButton->isHidden())
    {
        ui->axisXToolButton->click();
    }
    else if(!ui->axisYToolButton->isHidden())
    {
        ui->axisYToolButton->click();
    }
    else if(!ui->axisZToolButton->isHidden())
    {
        ui->axisZToolButton->click();
    }
    else if(!ui->axisPToolButton->isHidden())
    {
        ui->axisPToolButton->click();
    }
    else if(!ui->axisQToolButton->isHidden())
    {
        ui->axisQToolButton->click();
    }
    else if(!ui->axisAToolButton->isHidden())
    {
        ui->axisAToolButton->click();
    }
    else if(!ui->axisBToolButton->isHidden())
    {
        ui->axisBToolButton->click();
    }
    else if(!ui->axisCToolButton->isHidden())
    {        ui->axisCToolButton->click();
    }
    /***********BUG#202***************************/
    else
    {
        ui->structDefButton->click() ;
    }
    refreshTimer_.start(20);
    QWidget::showEvent(e);
}

void ICMachineStructPage::on_axisXToolButton_clicked()
{
    ShowSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisX1);
}

void ICMachineStructPage::on_axisYToolButton_clicked()
{
    HideSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisY1);
}

void ICMachineStructPage::on_axisZToolButton_clicked()
{
    ShowSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisZ);
}

void ICMachineStructPage::on_axisPToolButton_clicked()
{
    HideSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisX2);
}

void ICMachineStructPage::on_axisQToolButton_clicked()
{
    HideSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisY2);
}

void ICMachineStructPage::on_axisAToolButton_clicked()
{
    HideSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisA);
}

void ICMachineStructPage::on_axisBToolButton_clicked()
{
    HideSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisB);
}

void ICMachineStructPage::on_axisCToolButton_clicked()
{
    HideSafeZone();
    SetCurrentAxis(ICVirtualHost::ICAxis_AxisC);
}

//void ICMachineStructPage::on_axisDefineToolButton_clicked()
//{
//    ui->content->setCurrentIndex(1);
//}

void ICMachineStructPage::on_structDefButton_clicked()
{
    if(structPage_ == NULL)
    {
        structPage_ = new ICStructDefineFrame();
        ui->content->addWidget(structPage_);
        connect(structPage_,
                SIGNAL(StructChanged()),
                this,
                SLOT(UpdateAxisDefine_()));
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
    ui->minUnitLabel->setText(tr("mm"));
    ui->maxUnitLabel->setText(tr("mm"));
    ui->label_3->setText(tr("mm"));
    ui->label_4->setText(tr("mm"));
    ui->label_11->setText(tr("mm"));
    ui->label->setText(tr("Mechanical length"));
    ui->label_8->setText(tr("Distance/Rotation"));
    rotateValidator_->setTop(32767);
//    maximumValidator_->setTop(65530);
    maximumValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    minSecValidator_->setBottom(0);
    maxSecValidator_->setBottom(0);

    maximumValidator_->setBottom(0);
//    ui->maximumDisplacementLineEdit->setValidator(maximumValidator_);
    ui->mechanicalLengthLineEdit->setValidator(intValidator);
    intValidator->setTop(32767);
    maxMoveValidator_->setBottom(0);
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
        ui->label_2->setText(tr("Maximum displacement"));
        intValidator->setTop(32760);
        maxMoveValidator_->setBottom(-32760);

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
        ui->label_2->setText(tr("Maximum displacement"));
//        minSecValidator_->setBottom(10);
//        maxSecValidator_->setBottom(500);
        maxMoveValidator_->setBottom(-32760);
         intValidator->setTop(32760);

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
        ui->label_2->setText(tr("Maximum displacement"));
        maxMoveValidator_->setBottom(-3600);
        intValidator->setTop(3600);
        ui->minUnitLabel->setText(tr("deg"));
        ui->maxUnitLabel->setText(tr("deg"));
        ui->label_3->setText(tr("deg"));
        ui->label_4->setText(tr("deg"));
        ui->label_11->setText(tr("deg"));

        ui->minLabel->setText(minText);
        ui->maxLabel->setText(maxText);

    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        machineLangth = ICVirtualHost::SYS_P_Length;
        maxLangth = ICVirtualHost::SYS_P_Maxium;
        iSafe = ICVirtualHost::SYS_P_InSafe;
        oSafe = ICVirtualHost::SYS_P_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("P");
        minText = tr("Min pos inside mold");
        maxText = tr("Max pos inside mold");
        ui->distanceRotationEdit->SetDecimalPlaces(2);
        maxMoveValidator_->setBottom(-3600);
         intValidator->setTop(3600);
        ui->label_2->setText(tr("Maximum displacement"));
        ui->minUnitLabel->setText(tr("deg"));
        ui->maxUnitLabel->setText(tr("deg"));
        ui->label_3->setText(tr("deg"));
        ui->label_4->setText(tr("deg"));
        ui->label_11->setText(tr("deg"));
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        machineLangth = ICVirtualHost::SYS_Q_Length;
        maxLangth = ICVirtualHost::SYS_Q_Maxium;
        iSafe = ICVirtualHost::SYS_Q_InSafe;
        oSafe = ICVirtualHost::SYS_Q_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Q");
        minText = tr("Max standby pos");
        maxText = tr("Leave origin pos");
        ui->distanceRotationEdit->SetDecimalPlaces(2);
        ui->label_2->setText(tr("Maximum displacement"));
        minSecValidator_->setBottom(10);
        maxSecValidator_->setBottom(500);
        maxMoveValidator_->setBottom(-3600);
         intValidator->setTop(3600);
        ui->minUnitLabel->setText(tr("deg"));
        ui->maxUnitLabel->setText(tr("deg"));
        ui->label_3->setText(tr("deg"));
        ui->label_4->setText(tr("deg"));
        ui->label_11->setText(tr("deg"));

    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        machineLangth = ICVirtualHost::SYS_A_Length;
        maxLangth = ICVirtualHost::SYS_A_Maxium;
        iSafe = ICVirtualHost::SYS_A_InSafe;
        oSafe = ICVirtualHost::SYS_A_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("A");
//        minText = tr("Internal security zone");
//        maxText = tr("External security zone");
        minText = tr("Transever security zone(Less)");
        maxText = tr("Transever security zone(Lagger)");
        ui->minUnitLabel->setText(tr("degree"));
        ui->maxUnitLabel->setText(tr("degree"));
        ui->label_3->setText(tr("degree"));
        ui->label_4->setText(tr("degree"));
        ui->label_11->setText(tr("degree"));
        ui->label->setText(tr("Max Rotate"));
        ui->label_8->setText(tr("Machine Per"));
        ui->distanceRotationEdit->SetDecimalPlaces(1);
        rotateValidator_->setTop(3600);
        intValidator->setTop(3600);
        ui->mechanicalLengthLineEdit->setValidator(new QIntValidator(0,3600,this));
//        maximumValidator_->setTop(900);
//        maximumValidator_->setBottom(-900);
        ui->maximumDisplacementLineEdit->setValidator(originValidator_);
        ui->label_2->setText(tr("Origin Offset"));
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        machineLangth = ICVirtualHost::SYS_B_Length;
        maxLangth = ICVirtualHost::SYS_B_Maxium;
        iSafe = ICVirtualHost::SYS_B_InSafe;
        oSafe = ICVirtualHost::SYS_B_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("B");
//        minText = tr("Internal security zone");
//        maxText = tr("External security zone");
        minText = tr("Transever security zone(Less)");
        maxText = tr("Transever security zone(Lagger)");
        ui->minUnitLabel->setText(tr("degree"));
        ui->maxUnitLabel->setText(tr("degree"));
        ui->label_3->setText(tr("degree"));
        ui->label_4->setText(tr("degree"));
        ui->label_11->setText(tr("degree"));
        ui->label->setText(tr("Max Rotate"));
        ui->label_8->setText(tr("Machine Per"));
        ui->distanceRotationEdit->SetDecimalPlaces(1);
        rotateValidator_->setTop(3600);
        intValidator->setTop(3600);
        ui->mechanicalLengthLineEdit->setValidator(new QIntValidator(0,3600,this));
//        maximumValidator_->setTop(900);
//        maximumValidator_->setBottom(-900);
        ui->maximumDisplacementLineEdit->setValidator(originValidator_);
        ui->label_2->setText(tr("Origin Offset"));
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        machineLangth = ICVirtualHost::SYS_C_Length;
        maxLangth = ICVirtualHost::SYS_C_Maxium;
        iSafe = ICVirtualHost::SYS_C_InSafe;
        oSafe = ICVirtualHost::SYS_C_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("C");
//        minText = tr("Transever security zone(Less)");
//        maxText = tr("Transever security zone(Lagger)");
        ui->distanceRotationEdit->SetDecimalPlaces(1);
        rotateValidator_->setTop(3600);
        intValidator->setTop(3600);
        ui->mechanicalLengthLineEdit->setValidator(new QIntValidator(0,3600,this));
//        maximumValidator_->setTop(900);
//        maximumValidator_->setBottom(-900);
        ui->maximumDisplacementLineEdit->setValidator(originValidator_);
        ui->label_2->setText(tr("Origin Offset"));
        ui->minUnitLabel->setText(tr("degree"));
        ui->maxUnitLabel->setText(tr("degree"));
        ui->label_3->setText(tr("degree"));
        ui->label_4->setText(tr("degree"));
        ui->label_11->setText(tr("degree"));
        ui->label->setText(tr("Max Rotate")); //最大旋转
        ui->label_8->setText(tr("Machine Per")); //电机每圈
    }
    else
    {
        return;
    }
    QMap<QObject*, int>::iterator p = editorToConfigIDs_.begin();
    while(p != editorToConfigIDs_.end())
    {
        p.key()->blockSignals(true);
        ++p;
    }

    ui->mechanicalLengthLineEdit->SetThisIntToThisText(qint16(host->SystemParameter(machineLangth).toInt()));
    ui->maximumDisplacementLineEdit->SetThisIntToThisText(qint16(host->SystemParameter(maxLangth).toInt()));
    ui->internalSecurityZoneLineEdit->SetThisIntToThisText(qint16(host->SystemParameter(iSafe).toInt()));
    ui->externalSecurityZoneLineEdit->SetThisIntToThisText(qint16(host->SystemParameter(oSafe).toInt()));
    QString format = QString("%.%1f").arg(ui->distanceRotationEdit->DecimalPlaces());
    ui->distanceRotationEdit->setText(QString().sprintf(format.toAscii(), total));
    //    ui->distanceRotationEdit->SetThisIntToThisText(total);
    /*********BUG#186.同791行一起**************/
    intValidator->setBottom(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());
    maxMoveValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());

    minSecValidator_->setBottom(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());
    minSecValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    maxSecValidator_->setBottom(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());
    maxSecValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());


    p = editorToConfigIDs_.begin();
    while(p != editorToConfigIDs_.end())
    {
        p.key()->blockSignals(false);
        ++p;
    }
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
#ifdef Q_WS_QWS
    if(processor->ExecuteCommand(command).toBool())
#endif
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
        ICMessageBox::ICWarning(this, tr("Information"), tr("Save Successfully!"));
        //        UpdateAxisDefine_();
        ICModifyFrame::Instance()->OnActionTriggered(ICConfigString::kCS_AXIS_Config_Save, tr("Save"), tr(""));

    }
}

void ICMachineStructPage::InitInterface()
{
    //QIntValidator * intValidator = new QIntValidator(0, 65530, this);
    minSecValidator_ = new QIntValidator(0,0,this);
    maxSecValidator_ = new QIntValidator(0, 0, this);
    ui->mechanicalLengthLineEdit->SetDecimalPlaces(1);
    intValidator = new QIntValidator(0, 65530, this);
    ui->mechanicalLengthLineEdit->setValidator(intValidator);
//    ui->mechanicalLengthLineEdit->setValidator(new QIntValidator(-32760, 32760, this));
    maxMoveValidator_ = new QIntValidator(0, 65530, this);
    originValidator_ = new QIntValidator(-900, 900, this);
    ui->maximumDisplacementLineEdit->SetDecimalPlaces(1);
    maximumValidator_ = new QIntValidator(0, 65530, this);
//    ui->maximumDisplacementLineEdit->setValidator(new QIntValidator(-32760, 32760, this));
     ui->maximumDisplacementLineEdit->setValidator(maxMoveValidator_);
  //  externalValidator_ = new QIntValidator(0, 65530, this);
    ui->internalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->internalSecurityZoneLineEdit->setValidator(minSecValidator_);
    ui->externalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->externalSecurityZoneLineEdit->setValidator(maxSecValidator_);
    rotateValidator_ = new QIntValidator(0, 65530, this);
    ui->distanceRotationEdit->SetDecimalPlaces(2);
    ui->distanceRotationEdit->setValidator(rotateValidator_);
}

void ICMachineStructPage::HideSafeZone()
{
    ui->minLabel->hide();
    ui->minUnitLabel->hide();
    ui->internalSecurityZoneLineEdit->hide();
    ui->maxLabel->hide();
    ui->maxUnitLabel->hide();
    ui->externalSecurityZoneLineEdit->hide();
}

void ICMachineStructPage::ShowSafeZone()
{
    ui->minLabel->show();
    ui->minUnitLabel->show();
    ui->internalSecurityZoneLineEdit->show();
    ui->maxLabel->show();
    ui->maxUnitLabel->show();
    ui->externalSecurityZoneLineEdit->show();
}

void ICMachineStructPage::UpdateAxisDefine_()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int tmpAxis = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
    if(axisDefine_ != tmpAxis)
    {
        axisDefine_ = tmpAxis;
        ui->axisXToolButton->hide();
        ui->axisYToolButton->hide();
        ui->axisZToolButton->hide();
        ui->axisPToolButton->hide();
        ui->axisQToolButton->hide();
        ui->axisAToolButton->hide();
        ui->axisBToolButton->hide();
        ui->axisCToolButton->hide();

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisXToolButton->hide();
        }
        else
        {
            ui->axisXToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisYToolButton->hide();
        }
        else
        {
            ui->axisYToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisZToolButton->hide();
        }
        else
        {
            ui->axisZToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisPToolButton->hide();
        }
        else
        {
            ui->axisPToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisQToolButton->hide();
        }
        else
        {
            ui->axisQToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisAToolButton->hide();
        }
        else
        {
            ui->axisAToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisBToolButton->hide();
        }
        else
        {
            ui->axisBToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) != ICVirtualHost::ICAxisDefine_Servo)
        {
            ui->axisCToolButton->hide();
        }
        else
        {
            ui->axisCToolButton->show();
        }
    }
}

void ICMachineStructPage::on_testPushButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int addCmd;
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        addCmd = IC::CMD_TestX;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        addCmd = IC::CMD_TestY;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        addCmd = IC::CMD_TestZ;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        addCmd = IC::CMD_TestX2;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        addCmd = IC::CMD_TestY2;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        addCmd = IC::CMD_TestA;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        addCmd = IC::CMD_TestB;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        addCmd = IC::CMD_TestC;
    }
    processor->ExecuteHCCommand(addCmd, 0);
}

void ICMachineStructPage::on_revTestPushButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int addCmd;
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        addCmd = IC::CMD_TestxRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        addCmd = IC::CMD_TestyRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        addCmd = IC::CMD_TestzRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        addCmd = IC::CMD_TestX2Rev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        addCmd = IC::CMD_TestY2Rev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        addCmd = IC::CMD_TestARev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        addCmd = IC::CMD_TestBRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        addCmd = IC::CMD_TestCRev;
    }
    else
    {
        return;
    }
    processor->ExecuteHCCommand(addCmd, 0);
}

void ICMachineStructPage::on_pushButton_clicked()
{
     ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TestClr, 0);
}

void ICMachineStructPage::StatusRefresh()
{
    QString pos;
    QString feedbackPos;
    QString zSignal;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        pos = host->HostStatus(ICVirtualHost::XPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgX0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgX1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        pos = host->HostStatus(ICVirtualHost::YPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgY0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgY1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        pos = host->HostStatus(ICVirtualHost::ZPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgZ0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgZ1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        pos = host->HostStatus(ICVirtualHost::PPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgP0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgP1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        pos = host->HostStatus(ICVirtualHost::QPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgQ0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgQ1).toString();
    }
//    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
//    {
//        pos = host->HostStatus(ICVirtualHost::APos).toString();
//        feedbackPos = host->HostStatus(ICVirtualHost::DbgA0).toString();
//        zSignal = host->HostStatus(ICVirtualHost::DbgA1).toString();
//    }
//    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
//    {
//        pos = host->HostStatus(ICVirtualHost::BPos).toString();
//        feedbackPos = host->HostStatus(ICVirtualHost::DbgB0).toString();
//        zSignal = host->HostStatus(ICVirtualHost::DbgB1).toString();
//    }
//    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
//    {
//        pos = host->HostStatus(ICVirtualHost::CPos).toString();
//        feedbackPos = host->HostStatus(ICVirtualHost::DbgC0).toString();
//        zSignal = host->HostStatus(ICVirtualHost::DbgC1).toString();
//    }

    ui->testLineEdit->setText(pos);
    ui->feedbackEdit->setText(feedbackPos);
    ui->zSignalEdit->setText(zSignal);
}

void ICMachineStructPage::on_mechanicalLengthLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    maxMoveValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    maximumValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    minSecValidator_->setTop(maximumValidator_->top());
    maxSecValidator_->setTop(maximumValidator_->top());
 //   externalValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
}

/*********BUG#186*当maximumDisplacementLine内容改变时设置范围的最低值******/
void ICMachineStructPage::on_maximumDisplacementLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    intValidator->setBottom(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());
    minSecValidator_->setBottom(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());
    maxSecValidator_->setBottom(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());

}

void ICMachineStructPage::OnConfigChanged(QObject *w, const QString& newV, const QString& oldV)
{
    ICModifyFrame::Instance()->OnActionTriggered(editorToConfigIDs_.value(w, ICConfigString::kCS_Err) + currentAxis_,
                                                newV,
                                                oldV);
}

void ICMachineStructPage::OnConfigChanged(const QString &text)
{
    ICLineEditWithVirtualNumericKeypad* edit = qobject_cast<ICLineEditWithVirtualNumericKeypad*>(sender());
    OnConfigChanged(edit, text, edit->LastValue());
}

void ICMachineStructPage::OnConfigChanged(int v, int ov)
{
    ICButtonGroup* icbg = qobject_cast<ICButtonGroup*>(sender());
    QButtonGroup* bg = icbg->ButtongGroup();
    OnConfigChanged(bg, bg->button(v)->text(), bg->button(ov)->text());
}

void ICMachineStructPage::OnConfigChanged(int v)
{
    ICComboBox* edit = qobject_cast<ICComboBox*>(sender());
    OnConfigChanged(edit, edit->currentText(), edit->itemText(edit->LastValue()));
}

void ICMachineStructPage::OnConfigChanged(bool b)
{
    QCheckBox* edit = qobject_cast<QCheckBox*>(sender());
    QString oldVal = b ? QObject::tr("Not Used") : QObject::tr("Used");
    QString newVal = b ? QObject::tr("Used") : QObject::tr("Not Used");
    OnConfigChanged(edit, newVal, oldVal);
}

void ICMachineStructPage::on_servoToolButton_clicked()
{
    if(servoPage_ == NULL)
    {
        servoPage_ = new ICMachineConfigPage();
        ui->content->addWidget(servoPage_);
    }
    ui->content->setCurrentWidget(servoPage_);
}
