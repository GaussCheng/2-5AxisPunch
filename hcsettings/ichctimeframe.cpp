#include "ichctimeframe.h"
#include "ui_ichctimeframe.h"
#include "iclineeditwrapper.h"
#include "icvirtualhost.h"
#include "icmold.h"
#include "icmodifyframe.h"

#include <QDebug>

ICHCTimeFrame::ICHCTimeFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCTimeFrame)
{
    ui->setupUi(this);

//    ui->mainArmForwardEdit->hide();

//    ui->mainArmBackwardEdit->hide();

    ui->secondaryArmDownLineEdit->hide();

    ui->secondaryArmUpLineEdit->hide();
    ui->secondayGoBackwardLineEdit->hide();
    ui->secondayGoForwardLineEdit->hide();
//    ui->mainArmUpEdit->hide();
//    ui->mainArmDownEdit->hide();
//    ui->horizontal2LineEdit->hide();
    ui->vertical2LineEdit->hide();
    ui->horizontalLineEdit->hide();
//    ui->verticalineEdit->hide();


    ui->label_10->hide();
    ui->horizontal2LineEdit->hide();
    ui->label_28->hide();
    InitInterFace();
}

ICHCTimeFrame::~ICHCTimeFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void ICHCTimeFrame::InitInterFace()
{
    QIntValidator * validator = new QIntValidator(0, 3000, this);
    QLayout * layout = ui->gridLayout;
    ICLineEditWithVirtualNumericKeypad *lineEdit;
    for(int i = 0; i != layout->count(); ++i)
    {
        lineEdit = qobject_cast<ICLineEditWithVirtualNumericKeypad*>(layout->itemAt(i)->widget());
        if(lineEdit != NULL)
        {
            lineEdit->SetDecimalPlaces(1);
            lineEdit->setValidator(validator);
        }
    }

    BindingParam_(ui->horizontalLineEdit, ICVirtualHost::SM_TIMEHORI);
    BindingParam_(ui->verticalineEdit, ICVirtualHost::SM_TIMEHORI);
    BindingParam_(ui->horizontal2LineEdit, ICVirtualHost::SM_TIMEVERT);
    BindingParam_(ui->vertical2LineEdit, ICVirtualHost::SM_TIMEVERT2);
    BindingParam_(ui->secondaryArmDownLineEdit, ICVirtualHost::SM_TIMEVDOWN);
    BindingParam_(ui->secondaryArmUpLineEdit, ICVirtualHost::SM_TIMEVUP);
    BindingParam_(ui->secondayGoForwardLineEdit, ICVirtualHost::SM_TIMEVFOR);
    BindingParam_(ui->secondayGoBackwardLineEdit, ICVirtualHost::SM_TIMEVBACK);
    BindingParam_(ui->mainArmForwardEdit, ICVirtualHost::SM_TIMEMFOR);
    BindingParam_(ui->mainArmBackwardEdit, ICVirtualHost::SM_TIMEMBACK);
    BindingParam_(ui->mainArmUpEdit, ICVirtualHost::SM_TIMEMUP);
    BindingParam_(ui->mainArmDownEdit, ICVirtualHost::SM_TIMEMDOWN);

    editorToConfigIDs_.insert(ui->verticalineEdit, ICConfigString::kCS_TIME_Ver1);
    editorToConfigIDs_.insert(ui->horizontalLineEdit, ICConfigString::kCS_TIME_Hor1);
    editorToConfigIDs_.insert(ui->vertical2LineEdit, ICConfigString::kCS_TIME_Ver2);
    editorToConfigIDs_.insert(ui->horizontal2LineEdit, ICConfigString::kCS_TIME_Hor2);
    editorToConfigIDs_.insert(ui->mainArmUpEdit, ICConfigString::kCS_TIME_MArm_UP);
    editorToConfigIDs_.insert(ui->mainArmDownEdit, ICConfigString::kCS_TIME_MArm_Dw);
    editorToConfigIDs_.insert(ui->mainArmForwardEdit, ICConfigString::kCS_TIME_MArm_FW);
    editorToConfigIDs_.insert(ui->mainArmBackwardEdit, ICConfigString::kCS_TIME_MArm_BW);
    editorToConfigIDs_.insert(ui->secondaryArmUpLineEdit, ICConfigString::kCS_TIME_SArm_UP);
    editorToConfigIDs_.insert(ui->secondaryArmDownLineEdit, ICConfigString::kCS_TIME_SArm_DW);
    editorToConfigIDs_.insert(ui->secondayGoForwardLineEdit, ICConfigString::kCS_TIME_SArm_FW);
    editorToConfigIDs_.insert(ui->secondayGoBackwardLineEdit, ICConfigString::kCS_TIME_SArm_BW);
    ICLogInit

}

void ICHCTimeFrame::BindingParam_(QLineEdit *edit, int addr)
{
    wrappers_.append(new ICLineEditWrapper(edit,
                                           addr,
                                           ICLineEditWrapper::System,
                                           ICLineEditWrapper::OneFraction));
}

void ICHCTimeFrame::hideEvent(QHideEvent *e)
{
    qDebug("time hide");
    if(ICVirtualHost::GlobalVirtualHost()->IsParamChanged())
    {
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
//    ICMold::CurrentMold()->SaveMoldParamsFile();
    QFrame::hideEvent(e);
}

void ICHCTimeFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

ICLogFunctions(ICHCTimeFrame)
