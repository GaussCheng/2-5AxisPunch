#include <QDateTime>
#include "icprogramheadframe.h"
#include "ui_icprogramheadframe.h"

#include "icparameterssave.h"

ICProgramHeadFrame * ICProgramHeadFrame::instance_ = NULL;

ICProgramHeadFrame::ICProgramHeadFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICProgramHeadFrame),
    autoMin_(0)
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
    ui->moldNameLabel->setText(moldName);
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

void ICProgramHeadFrame::UpdateAutoTime()
{
    ui->autoTimeLabel->setText(QString(tr("%1 h")).arg(autoMin_ / qreal(60), 0, 'g', 1));
    ++autoMin_;
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
    connect(&autoTime_,
            SIGNAL(timeout()),
            this,
            SLOT(UpdateAutoTime()));
}

int ICProgramHeadFrame::CurrentLevel() const
{
    return ui->passwdLevelLabel->CurrentLevel();
}

void ICProgramHeadFrame::SetCurrentLevel(int level)
{
    ui->passwdLevelLabel->PasswdLevelChenged(level);
}
