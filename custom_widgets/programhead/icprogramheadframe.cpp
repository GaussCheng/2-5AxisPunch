#include <QDateTime>
#include "icprogramheadframe.h"
#include "ui_icprogramheadframe.h"
#include <QMessageBox>

#include "icparameterssave.h"

ICProgramHeadFrame * ICProgramHeadFrame::instance_ = NULL;

ICProgramHeadFrame::ICProgramHeadFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICProgramHeadFrame)
{
    ui->setupUi(this);
    pageTimer_.start(60000);
    UpdateDateTime();
    InitSignal();
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
    ui->currentDateLabel->setText(dateTime.toString("yyyy/MM/dd"));

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




