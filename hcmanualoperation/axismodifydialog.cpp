#include "axismodifydialog.h"
#include "ui_axismodifydialog.h"
#include "icvirtualhost.h"

AxisModifyDialog::AxisModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AxisModifyDialog)
{
    ui->setupUi(this);
    ui->xPos->SetDecimalPlaces(1);
    ui->yPos->SetDecimalPlaces(1);
    xValidator_ = new QIntValidator(-32760, 32760, this);
    yValidator_ = new QIntValidator(-32760, 32760, this);
    ui->xPos->setValidator(xValidator_);
    ui->yPos->setValidator(yValidator_);
#ifdef HC_AXIS_COUNT_5
    ui->zPos->SetDecimalPlaces(1);
    zValidator_ = new QIntValidator(-32760, 32760, this);
    ui->zPos->setValidator(zValidator_);

    ui->pPos->SetDecimalPlaces(1);
    pValidator_ = new QIntValidator(-32760, 32760, this);
    ui->pPos->setValidator(pValidator_);

    ui->tPos->SetDecimalPlaces(1);
    tValidator_ = new QIntValidator(-32760, 32760, this);
    ui->tPos->setValidator(tValidator_);
#endif

    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->pPos->hide();
    ui->tPos->hide();
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
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 6), ui->xPos->TransThisTextToThisInt());
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 6 + 1), ui->yPos->TransThisTextToThisInt());
#ifdef HC_AXIS_COUNT_5
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 6 + 2), ui->zPos->TransThisTextToThisInt());
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 6 + 3), ui->pPos->TransThisTextToThisInt());
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(point_.pointID * 6 + 4), ui->tPos->TransThisTextToThisInt());

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
    xValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_X_Maxium).toInt(),
                          host->SystemParameter(ICVirtualHost::SYS_X_Length).toInt());
    yValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_Y_Maxium).toInt(),
                          host->SystemParameter(ICVirtualHost::SYS_Y_Length).toInt());
    ui->xPos->SetThisIntToThisText(point.x);
    ui->yPos->SetThisIntToThisText(point.y);
#ifdef HC_AXIS_COUNT_5
    zValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_Z_Maxium).toInt(),
                          host->SystemParameter(ICVirtualHost::SYS_Z_Length).toInt());
    ui->zPos->SetThisIntToThisText(point.z);

    pValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_P_Maxium).toInt(),
                          host->SystemParameter(ICVirtualHost::SYS_P_Length).toInt());
    ui->pPos->SetThisIntToThisText(point.p);

    tValidator_->setRange(host->SystemParameter(ICVirtualHost::SYS_Q_Maxium).toInt(),
                          host->SystemParameter(ICVirtualHost::SYS_Q_Length).toInt());
    ui->tPos->SetThisIntToThisText(point.q);
#endif
    this->exec();
}

void AxisModifyDialog::on_setButton_clicked()
{
    if(point_.pointID < 0)
    {
        return;
    }
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ui->xPos->SetThisIntToThisText(host->GetActualPos(ICVirtualHost::ICAxis_AxisX1));
    ui->yPos->SetThisIntToThisText(host->GetActualPos(ICVirtualHost::ICAxis_AxisY1));
//    ui->xPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::XPos).toInt());
//    ui->yPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::YPos).toInt());
#ifdef HC_AXIS_COUNT_5
//    ui->zPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::ZPos).toInt());
    ui->zPos->SetThisIntToThisText(host->GetActualPos(ICVirtualHost::ICAxis_AxisZ));
    ui->pPos->SetThisIntToThisText(host->GetActualPos(ICVirtualHost::ICAxis_AxisX2));
    ui->tPos->SetThisIntToThisText(host->GetActualPos(ICVirtualHost::ICAxis_AxisY2));
#endif
}
