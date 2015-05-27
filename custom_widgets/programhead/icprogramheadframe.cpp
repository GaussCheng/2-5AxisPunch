#include <QDateTime>
#include "icprogramheadframe.h"
#include "ui_icprogramheadframe.h"
#include <QMessageBox>

#include "icparameterssave.h"
#include "operatingratiosetdialog.h"
#include "iccommandprocessor.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"

ICProgramHeadFrame * ICProgramHeadFrame::instance_ = NULL;
static QMap<int, QString> statusToStringMap;
ICProgramHeadFrame::ICProgramHeadFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICProgramHeadFrame)
{
    ui->setupUi(this);
    currentStatus_ = -1;
    isRobotOrigin_ = false;
    isPunOrigin_ = false;
    isControled_ = false;
    statusToStringMap.insert(0, tr("Stop"));
    statusToStringMap.insert(1, tr("Manual"));
    statusToStringMap.insert(2, tr("Auto"));
    pageTimer_.start(60000);
    UpdateDateTime();
    InitSignal();
    on = QPixmap(":/resource/ledgreen(16).png");
    off = QPixmap(":/resource/ledgray(16).png");
    connect(ui->moldNameLabel,
            SIGNAL(clicked()),
            SIGNAL(MoldButtonClicked()));
    ui->moldNameLabel->setEnabled(false);
    ui->handSelect->setCurrentIndex(-1);
//    ChangeCurrentStatus(0);
}

ICProgramHeadFrame::~ICProgramHeadFrame()
{
    delete ui;
}

void ICProgramHeadFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        statusToStringMap.insert(0, tr("Stop"));
        statusToStringMap.insert(1, tr("Manual"));
        statusToStringMap.insert(2, tr("Auto"));
        ui->retranslateUi(this);
        SetCurrentMoldName(currentMoldName_);
        UpdateDateTime();
        break;
    default:
        break;
    }
}

void ICProgramHeadFrame::SetCurrentMoldName(const QString & moldName)
{
    currentMoldName_ = moldName;
    if(moldName.endsWith(".act"))
    {
        ui->moldNameLabel->setText(moldName.left(moldName.size() - 4));
    }
    else
    {
        ui->moldNameLabel->setText(moldName);
    }
}


//void ICProgramHeadFrame::SetCurrentCategoryName(const QString & categoryName)
//{
//    ui->currentPageLabel->setText(categoryName);
//}

void ICProgramHeadFrame::UpdateDateTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    ui->currentTimeLabel->setText(dateTime.toString("hh:mm\nyyyy/MM/dd"));
//    ui->currentDateLabel->setText(dateTime.toString("yyyy/MM/dd"));

}

void ICProgramHeadFrame::InitSignal()
{
    connect(&pageTimer_,
            SIGNAL(timeout()),
            this,
            SLOT(UpdateDateTime()));
    connect(ui->passwdLevelLabel,
            SIGNAL(Levelchenged(int)),
            SLOT(OnLevelChanged(int)));
}

int ICProgramHeadFrame::CurrentLevel() const
{
    return ui->passwdLevelLabel->CurrentLevel();
}

void ICProgramHeadFrame::SetCurrentLevel(int level)
{
    ui->passwdLevelLabel->PasswdLevelChenged(level);
//    if(level)
}


void ICProgramHeadFrame::ChangeCurrentStatus(int status)
{
    if(!statusToStringMap.contains(status)) return;
    if(status != this->currentStatus_)
    {
        currentStatus_ = status;
        ui->statusLabel->setText(statusToStringMap.value(status));
    }
}

void ICProgramHeadFrame::ChangeControlStatus(bool isControled)
{
    if(isControled != this->isControled_)
    {
        this->isControled_ = isControled;
        if(isControled){
            CanConfig canConfig;
            canConfig.all = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Resv1).toInt();
            if(canConfig.b.canType == 1){
                ui->controlStatus->setText(tr("Host"));
            }
            else if(canConfig.b.canType == 2){
                ui->controlStatus->setText(tr("Aux%1").arg(canConfig.b.canAddr));
            }
        }
        else{
            ui->controlStatus->setText(tr("Single"));
        }
    }

}

void ICProgramHeadFrame::ChangeRobotOrigin(bool isOrigin)
{
    if(isOrigin != this->isRobotOrigin_)
    {
         isRobotOrigin_ = isOrigin;
         if(isRobotOrigin_)
         {
             ui->robotOrigin->setPixmap(on);
         }
         else
         {
             ui->robotOrigin->setPixmap(off);
         }
    }
}

void ICProgramHeadFrame::ChangePunchOrigin(bool isOrigin)
{
    if(isOrigin != this->isPunOrigin_)
    {
         isPunOrigin_ = isOrigin;
         if(isPunOrigin_)
         {
             ui->punchOrigin->setPixmap(on);
         }
         else
         {
             ui->punchOrigin->setPixmap(off);
         }
    }
}

void ICProgramHeadFrame::OnLevelChanged(int level)
{
    ui->moldNameLabel->setEnabled(level >= 1);
    emit LevelChanged(level);
}

void ICProgramHeadFrame::ChangStatusmoldNameLabelOperation(bool s)
{
    int level = ICProgramHeadFrame::CurrentLevel();
    ui->moldNameLabel->setEnabled((level >= 1)&s);
}

void ICProgramHeadFrame::on_handSelect_currentIndexChanged(int index)
{
//    OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis();
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_BIASX + index);
}

void ICProgramHeadFrame::ChangeHandSelect(int index)
{
    ui->handSelect->blockSignals(true);
    ui->handSelect->setCurrentIndex(index);
    ui->handSelect->blockSignals(false);
}

void ICProgramHeadFrame::SetHanSelectEnable(bool en)
{
    if(en)
        ui->handSelect->setEnabled(true);
    else
    {
        ChangeHandSelect(-1);
        ui->handSelect->setEnabled(false);
    }
}

int ICProgramHeadFrame::HandSpeed()
{
    int index = ui->handSpeed->currentIndex();
    switch (index) {
    case 0:
        return 1;
    case 1:
        return 5;
    case 2:
        return 10;
    default:
        return 1;
    }
}
