#include "icsimplesetting.h"
#include "ui_icsimplesetting.h"
#include "icvirtualhost.h"
#include "icparameterssave.h"

ICSimpleSetting::ICSimpleSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICSimpleSetting)
{
    ui->setupUi(this);
    InitEditors();
}



ICSimpleSetting::~ICSimpleSetting()
{
    delete ui;
}



void ICSimpleSetting::showEvent(QShowEvent *e)
{
    ShowEditors();
    SetEditors();
    InitValidators();
}

void ICSimpleSetting::hideEvent(QHideEvent *e)
{
    SaveEditors();
}

void ICSimpleSetting::ShowEditors()
{

    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->xGroup->show();
    }
    else{
        ui->xGroup->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->yGroup->show();
    }
    else{
        ui->yGroup->hide();
    }

    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->sGroup->show();
    }
    else{
        ui->sGroup->hide();
    }

    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->rGroup->show();
    }
    else{
        ui->rGroup->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->tGroup->show();
    }
    else{
        ui->tGroup->hide();
    }
}

void ICSimpleSetting::InitEditors()
{
    editToAddrs.insert(ui->mechanicalLengthLineEdit,ICVirtualHost::SYS_X_Length);
    editToAddrs.insert(ui->mechanicalLengthLineEdit_3,ICVirtualHost::SYS_Y_Length);
    editToAddrs.insert(ui->mechanicalLengthLineEdit_2,ICVirtualHost::SYS_Z_Length);
    editToAddrs.insert(ui->mechanicalLengthLineEdit_4,ICVirtualHost::SYS_P_Length);
    editToAddrs.insert(ui->mechanicalLengthLineEdit_5,ICVirtualHost::SYS_Q_Length);

    editToAddrs.insert(ui->maximumDisplacementLineEdit,ICVirtualHost::SYS_X_Maxium);
    editToAddrs.insert(ui->maximumDisplacementLineEdit_3,ICVirtualHost::SYS_Y_Maxium);
    editToAddrs.insert(ui->maximumDisplacementLineEdit_2,ICVirtualHost::SYS_Z_Maxium);
    editToAddrs.insert(ui->maximumDisplacementLineEdit_4,ICVirtualHost::SYS_P_Maxium);
    editToAddrs.insert(ui->maximumDisplacementLineEdit_5,ICVirtualHost::SYS_Q_Maxium);

    editToAddrs.insert(ui->internalSecurityZoneLineEdit,ICVirtualHost::SYS_X_InSafe);
    editToAddrs.insert(ui->internalSecurityZoneLineEdit_2,ICVirtualHost::SYS_Z_InSafe);

    editToAddrs.insert(ui->externalSecurityZoneLineEdit,ICVirtualHost::SYS_X_OutSafe);
    editToAddrs.insert(ui->externalSecurityZoneLineEdit_2,ICVirtualHost::SYS_Z_OutSafe);

    rotateEdits << ui->distanceRotationEdit << ui->distanceRotationEdit_2 << ui->distanceRotationEdit_3
                << ui->distanceRotationEdit_4 << ui->distanceRotationEdit_5;


    rotateToAxis.insert(ui->distanceRotationEdit,"X");
    rotateToAxis.insert(ui->distanceRotationEdit_3,"Y");
    rotateToAxis.insert(ui->distanceRotationEdit_2,"Z");
    rotateToAxis.insert(ui->distanceRotationEdit_4,"P");
    rotateToAxis.insert(ui->distanceRotationEdit_5,"Q");

    rotateValidator = new QIntValidator(0,32767);

    foreach(ICLineEditWithVirtualNumericKeypad *editor,rotateEdits)
    {
        editor->setValidator(rotateValidator);
        editor->SetDecimalPlaces(1);
    }



    foreach(ICLineEditWithVirtualNumericKeypad *editor,editToAddrs.keys())
    {
        QIntValidator * v = new QIntValidator(-32760,32760);

        addrToValidators.insert(editToAddrs.value(editor),v);
        editor->setValidator(v);
        editor->SetDecimalPlaces(1);
        connect(editor,SIGNAL(textChanged(QString)),
                              this,SLOT(EditorTextChanged(QString)));
    }

    addrToValidators.value(ICVirtualHost::SYS_X_Length)->setRange(0,32760);
    addrToValidators.value(ICVirtualHost::SYS_Y_Length)->setRange(0,32760);
    addrToValidators.value(ICVirtualHost::SYS_Z_Length)->setRange(0,32760);
    addrToValidators.value(ICVirtualHost::SYS_P_Length)->setRange(0,32760);
    addrToValidators.value(ICVirtualHost::SYS_Q_Length)->setRange(0,32760);

}

void ICSimpleSetting::SetEditors()
{
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    foreach(ICLineEditWithVirtualNumericKeypad *editor,editToAddrs.keys())
    {

        editor->SetThisIntToThisText(qint16(host->SystemParameter(ICVirtualHost::ICSystemParameter(editToAddrs.value(editor))).toInt()));
    }
    foreach(ICLineEditWithVirtualNumericKeypad *editor,rotateEdits)
    {
        editor->SetThisIntToThisText(ICParametersSave::Instance()->DistanceRotation(rotateToAxis.value(editor)) * 10);
    }
}

void ICSimpleSetting::SaveEditors()
{

}

void ICSimpleSetting::InitValidators()
{
    addrToValidators.value(ICVirtualHost::SYS_X_Maxium)->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    addrToValidators.value(ICVirtualHost::SYS_Y_Maxium)->setTop(ui->mechanicalLengthLineEdit_3->TransThisTextToThisInt());
    addrToValidators.value(ICVirtualHost::SYS_Z_Maxium)->setTop(ui->mechanicalLengthLineEdit_2->TransThisTextToThisInt());
    addrToValidators.value(ICVirtualHost::SYS_P_Maxium)->setTop(ui->mechanicalLengthLineEdit_4->TransThisTextToThisInt());
    addrToValidators.value(ICVirtualHost::SYS_Q_Maxium)->setTop(ui->mechanicalLengthLineEdit_5->TransThisTextToThisInt());

    addrToValidators.value(ICVirtualHost::SYS_X_InSafe)->setRange(qint16(ui->maximumDisplacementLineEdit->TransThisTextToThisInt()),
                                                                                                                   qint16(ui->mechanicalLengthLineEdit->TransThisTextToThisInt()));
    addrToValidators.value(ICVirtualHost::SYS_X_OutSafe)->setRange(qint16(ui->maximumDisplacementLineEdit->TransThisTextToThisInt()),
                                                                                                                   qint16(ui->mechanicalLengthLineEdit->TransThisTextToThisInt()));
    addrToValidators.value(ICVirtualHost::SYS_Z_InSafe)->setRange(qint16(ui->maximumDisplacementLineEdit_2->TransThisTextToThisInt()),
                                                                                                                   qint16(ui->mechanicalLengthLineEdit_2->TransThisTextToThisInt()));
    addrToValidators.value(ICVirtualHost::SYS_Z_OutSafe)->setRange(qint16(ui->maximumDisplacementLineEdit_2->TransThisTextToThisInt()),
                                                                                                                   qint16(ui->mechanicalLengthLineEdit_2->TransThisTextToThisInt()));

}

void ICSimpleSetting::EditorTextChanged(QString text)
{
    Q_UNUSED(text);
    InitValidators();
}

