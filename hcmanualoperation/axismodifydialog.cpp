#include "axismodifydialog.h"
#include "ui_axismodifydialog.h"
#include "icvirtualhost.h"

AxisModifyDialog::AxisModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AxisModifyDialog)
{
    ui->setupUi(this);
    ui->xPos->SetDecimalPlaces(2);
    ui->yPos->SetDecimalPlaces(2);
    xValidator_ = new QIntValidator(-32760000, 32760000, this);
    yValidator_ = new QIntValidator(-32760000, 32760000, this);
    ui->xPos->setValidator(xValidator_);
    ui->yPos->setValidator(yValidator_);
#ifdef HC_AXIS_COUNT_5
    ui->zPos->SetDecimalPlaces(2);
    zValidator_ = new QIntValidator(-32760000, 32760000, this);
    ui->zPos->setValidator(zValidator_);

    ui->pPos->SetDecimalPlaces(2);
    pValidator_ = new QIntValidator(-32760000, 32760000, this);
    ui->pPos->setValidator(pValidator_);

    ui->tPos->SetDecimalPlaces(2);
    tValidator_ = new QIntValidator(-32760000, 32760000, this);
    ui->tPos->setValidator(tValidator_);
#endif
}

AxisModifyDialog::~AxisModifyDialog()
{
    delete ui;
}

void AxisModifyDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AxisModifyDialog::on_okButton_clicked()
{
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12), ui->xPos->TransThisTextToThisInt() & 0xFFFF);
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 1), ui->xPos->TransThisTextToThisInt() >> 16);

    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 2), ui->yPos->TransThisTextToThisInt() & 0xFFFF );
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 3), ui->yPos->TransThisTextToThisInt() >> 16);

#ifdef HC_AXIS_COUNT_5
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 4), ui->zPos->TransThisTextToThisInt() & 0xFFFF);
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 5), ui->zPos->TransThisTextToThisInt() >> 16);

    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 6), ui->pPos->TransThisTextToThisInt() & 0xFFFF);
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 7), ui->pPos->TransThisTextToThisInt() >> 16);

    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 8), ui->tPos->TransThisTextToThisInt() & 0xFFFF);
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 12 + 9), ui->tPos->TransThisTextToThisInt() >> 16);

#endif
    this->accept();
}

void AxisModifyDialog::on_cancelButton_clicked()
{
    this->reject();
}

void AxisModifyDialog::StartModify(ICPoint point)
{
    point_ = point;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();

    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->label->show();ui->xPos->show();ui->label_4->show();
    }
    else
    {
        ui->label->hide();ui->xPos->hide();ui->label_4->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->label_2->show();ui->yPos->show();ui->label_5->show();
    }
    else
    {
        ui->label_2->hide();ui->yPos->hide();ui->label_5->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->label_3->show();ui->zPos->show();ui->label_6->show();
    }
    else
    {
        ui->label_3->hide();ui->zPos->hide();ui->label_6->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->label_8->show();ui->pPos->show();ui->label_7->show();
    }
    else
    {
        ui->label_8->hide();ui->pPos->hide();ui->label_7->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->label_9->show();ui->tPos->show();ui->label_10->show();
    }
    else
    {
        ui->label_9->hide();ui->tPos->hide();ui->label_10->hide();
    }

    xValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_X_Maxium).toInt() * 10,
                          host->SystemParameter(ICVirtualHost::SYS_X_Length).toInt() * 10);
    yValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_Y_Maxium).toInt() * 10,
                          host->SystemParameter(ICVirtualHost::SYS_Y_Length).toInt() * 10);
    ui->xPos->SetThisIntToThisText(point.x);
    ui->yPos->SetThisIntToThisText(point.y);
#ifdef HC_AXIS_COUNT_5
    zValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_Z_Maxium).toInt() * 10,
                          host->SystemParameter(ICVirtualHost::SYS_Z_Length).toInt() * 10);
    ui->zPos->SetThisIntToThisText(point.z);

    pValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_P_Maxium).toInt() * 10,
                          host->SystemParameter(ICVirtualHost::SYS_P_Length).toInt() * 10);
    ui->pPos->SetThisIntToThisText(point.p);

    tValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_Q_Maxium).toInt() * 10,
                          host->SystemParameter(ICVirtualHost::SYS_Q_Length).toInt() * 10);
    ui->tPos->SetThisIntToThisText(point.q);
#endif
    this->exec();
}

quint32 AxisModifyDialog::GetPointValue(quint16 pos)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    quint32 s  = host->HostStatus(ICVirtualHost::DbgB0).toUInt() << 16;
    s = s + host->HostStatus(ICVirtualHost::DbgA1).toUInt();

    return ( (s >>( (pos -  ICVirtualHost::XPos)* 4 ) )& 0xF);
}

qint32 AxisModifyDialog::GetPosValue(ICVirtualHost::ICStatus status)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    qint16  p =  host->HostStatus(status).toInt() ;
    if(p < 0){
        qint32 v = p * 10 -  GetPointValue(status) ;
        return v;
    }
    else{
        qint32 v = p * 10 + GetPointValue(status) ;
        return v;
    }
}

void AxisModifyDialog::on_setButton_clicked()
{
    if(point_.pointID < 0)
    {
        return;
    }
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ui->xPos->SetThisIntToThisText(GetPosValue(ICVirtualHost::XPos));
    ui->yPos->SetThisIntToThisText(GetPosValue(ICVirtualHost::YPos));
//    ui->xPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::XPos).toInt());
//    ui->yPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::YPos).toInt());
#ifdef HC_AXIS_COUNT_5
//    ui->zPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::ZPos).toInt());
    ui->zPos->SetThisIntToThisText(GetPosValue(ICVirtualHost::ZPos));
    ui->pPos->SetThisIntToThisText(GetPosValue(ICVirtualHost::PPos));
    ui->tPos->SetThisIntToThisText(GetPosValue(ICVirtualHost::QPos));
#endif
}
