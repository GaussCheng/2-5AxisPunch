#include <QDateTime>
#include "icprogramheadframe.h"
#include "ui_icprogramheadframe.h"

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
    ui->moldNameLabel->setText(moldName);
}


void ICProgramHeadFrame::SetCurrentCategoryName(const QString & categoryName)
{
    ui->currentPageLabel->setText(categoryName);
}

void ICProgramHeadFrame::UpdateDateTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    ui->currentDateTimeLabel->setText(dateTime.toString("yyyy/MM/dd  hh:mm"));
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
