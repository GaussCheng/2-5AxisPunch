#include <QDateTime>
#include "icprogramheadframe.h"
#include "ui_icprogramheadframe.h"
#include <QMessageBox>

#include "icparameterssave.h"

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
    statusToStringMap.insert(0, tr("Stop"));
    statusToStringMap.insert(1, tr("Manual"));
    statusToStringMap.insert(2, tr("Auto"));
    pageTimer_.start(60000);
    UpdateDateTime();
    InitSignal();
    on = QPixmap(":/resource/ledgreen(16).png");
    off = QPixmap(":/resource/ledgray(16).png");

    ui->label_4->hide();
    ui->punchOrigin->hide();
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
    ui->currentTimeLabel->setText(dateTime.toString("hh:mm"));
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
            SIGNAL(LevelChanged(int)));
}

int ICProgramHeadFrame::CurrentLevel() const
{
    return ui->passwdLevelLabel->CurrentLevel();
}

void ICProgramHeadFrame::SetCurrentLevel(int level)
{
    ui->passwdLevelLabel->PasswdLevelChenged(level);
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

