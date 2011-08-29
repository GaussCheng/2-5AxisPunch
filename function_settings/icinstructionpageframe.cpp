#include <QKeyEvent>

#include "icinstructionpageframe.h"
#include "ui_icinstructionpageframe.h"

#include "ichcparametersframe.h"
#include "ichcinstructionpageframe.h"
#include "icmacroparameditorframe.h"

#include "ichcprogrammonitorframe.h"
#include "icvirtualhost.h"
#include "iccommandprocessor.h"
#include "icactioncommand.h"
#include "icmold.h"
#include "icmacrosubroutine.h"

#include "icprogramheadframe.h"
#include "ickeyboard.h"
#include "moldinformation.h"

#include <QDebug>

ICInstructionPageFrame *icInstructionPage = NULL;

ICInstructionPageFrame::ICInstructionPageFrame(QWidget *parent, QStackedLayout * functionBackPageLayout) :
        QFrame(parent),
        ui(new Ui::ICInstructionPageFrame),
        macroEditor_(NULL),
        programMonitor_(NULL),
        programEditor_(NULL),
        parameterEditor_(NULL)
{
    ui->setupUi(this);
    icInstructionPage = this;

    functionBackPageLayout_ = functionBackPageLayout;

    InitInstructionPage();
    UpdateTranslate();
    InitSignal();
}

ICInstructionPageFrame::~ICInstructionPageFrame()
{
    delete ui;
}

void ICInstructionPageFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        {
            ui->retranslateUi(this);
            UpdateTranslate();
        }
        break;
    default:
        break;
    }
}

void ICInstructionPageFrame::InitInstructionPage()
{
    if(functionBackPageLayout_ != NULL)
    {
        programMonitor_ = new ICHCProgramMonitorFrame;
        functionBackPageLayout_->addWidget(programMonitor_);
        programButtonToPage_.insert(ui->programMonitorWidget, programMonitor_);

//        parameterEditor_ = new ICHCParametersFrame;
//        functionBackPageLayout_->addWidget(parameterEditor_);
//        programButtonToPage_[ui->programParamWidget] = parameterEditor_;

//        programEditor_ = new ICHCInstructionPageFrame;
//        functionBackPageLayout_->addWidget(programEditor_);
//        programButtonToPage_[ui->programEditorWidget] = programEditor_;
    }

    ui->programMonitorWidget->hide();
//    ui->programMonitorWidget->setIcon(QIcon(":/resource/monitor.png"));
    ui->programEditorWidget->setIcon(QIcon(":/resource/editor.png"));
    ui->programParamWidget->setIcon(QIcon(":/resource/parameter.png"));

//    ui->programMonitorWidget->SetDirection(ICPageTag::IconOnRight);
    ui->programParamWidget->SetDirection(ICPageTag::IconOnRight);
    ui->programEditorWidget->SetDirection(ICPageTag::IconOnRight);
    ui->macroEiitorWidget->SetDirection(ICPageTag::IconOnRight);
}

void ICInstructionPageFrame::InitSignal()
{
    connect(ui->programParamWidget,
            SIGNAL(clicked()),
            this,
            SLOT(InstructionPageButtonClicked()));
    connect(ui->programEditorWidget,
            SIGNAL(clicked()),
            this,
            SLOT(InstructionPageButtonClicked()));
    connect(ui->programMonitorWidget,
            SIGNAL(clicked()),
            this,
            SLOT(InstructionPageButtonClicked()));
    connect(ui->macroEiitorWidget,
            SIGNAL(clicked()),
            this,
            SLOT(InstructionPageButtonClicked()));
    connect(MoldInformation::Instance(),
            SIGNAL(LoadFileInfoButtonClicked(QString)),
            this,
            SLOT(LoadFileInRecords(QString)));

//    connect(programEditor_,
//            SIGNAL(CurrentProgramChanged()),
//            programMonitor_,
//            SLOT(UpdateHostParam()));

//    connect(programEditor_,
//            SIGNAL(JumpToProgramEditorPage()),
//            this,
//            SLOT(ShowProgramEditorPage()));
}

void ICInstructionPageFrame::InstructionPageButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

//    if(clickedButton == ui->programEditorWidget)
//    {
//        ICCommandProcessor* process = ICCommandProcessor::Instance();
//        process->ExecuteHCCommand(IC::CMD_TurnTeach,
//                                  0,
//                                  ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
//                                  ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());

////        if(process->IsAlarmed())
////        {
////            return;
////        }
//    }
    Q_ASSERT_X(functionBackPageLayout_ != NULL, "ICInstructionPageFrame", "fuctionBackPageLayout is NULL");
    if(macroEditor_ == NULL && clickedButton == ui->macroEiitorWidget)
    {
        macroEditor_ = new ICMacroParamEditorFrame;
        functionBackPageLayout_->addWidget(macroEditor_);
        programButtonToPage_.insert(ui->macroEiitorWidget, macroEditor_);
    }
    else if(parameterEditor_ == NULL && clickedButton == ui->programParamWidget)
    {
        parameterEditor_ = new ICHCParametersFrame;
        functionBackPageLayout_->addWidget(parameterEditor_);
        programButtonToPage_.insert(ui->programParamWidget, parameterEditor_);
    }
    else if(programEditor_ == NULL && clickedButton == ui->programEditorWidget)
    {
        programEditor_ = new ICHCInstructionPageFrame;
        functionBackPageLayout_->addWidget(programEditor_);
        programButtonToPage_.insert(ui->programEditorWidget, programEditor_);
//        connect(programEditor_,
//                SIGNAL(JumpToProgramEditorPage()),
//                this,
//                SLOT(ShowProgramEditorPage()));
    }

    functionBackPageLayout_->setCurrentWidget(programButtonToPage_.value(clickedButton));

    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(clickedButton->text());
}

void ICInstructionPageFrame::ShowProgramMonitor()
{
    functionBackPageLayout_->setCurrentWidget(programMonitor_);
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(ui->programMonitorWidget->text());
}

void ICInstructionPageFrame::ShowProgramEditorPage()
{
    functionBackPageLayout_->setCurrentWidget(programEditor_);
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(ui->programEditorWidget->text());
}

void ICInstructionPageFrame::UpdateTranslate()
{
    ui->programParamWidget ->setText(tr("Program Parameter"));
    ui->programEditorWidget->setText(tr("Program Editor"));
    ui->programMonitorWidget->setText(tr("Program Monitor"));
    ui->macroEiitorWidget->setText(tr("Macro Editor"));
}

void ICInstructionPageFrame::LoadFileInRecords(const QString &name)
{
    if(programEditor_ == NULL)
    {
        programEditor_ = new ICHCInstructionPageFrame;
        functionBackPageLayout_->addWidget(programEditor_);
        programButtonToPage_.insert(ui->programEditorWidget, programEditor_);
    }
    ICHCInstructionPageFrame* edit = qobject_cast<ICHCInstructionPageFrame*>(programEditor_);
    edit->LoadFileInfoButtonClicked(name);
    ShowProgramEditorPage();
}
