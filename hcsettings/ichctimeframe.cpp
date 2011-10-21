#include "ichctimeframe.h"
#include "ui_ichctimeframe.h"
#include "iclineeditwrapper.h"
#include "icvirtualhost.h"
#include "icmold.h"

#include <QDebug>

ICHCTimeFrame::ICHCTimeFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCTimeFrame)
{
    ui->setupUi(this);

    InitInterFace();
}

ICHCTimeFrame::~ICHCTimeFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void ICHCTimeFrame::InitInterFace()
{
    QIntValidator * validator = new QIntValidator(0, 32767, this);
    /*ui->verticalineEdit->SetDecimalPlaces(1);
    ui->verticalineEdit->setValidator(validator);
    ui->horizontalLineEdit->SetDecimalPlaces(1);
    ui->horizontalLineEdit->setValidator(validator);
    ui->secondaryArmDownLineEdit->SetDecimalPlaces(1);
    ui->secondaryArmDownLineEdit->setValidator(validator);
    ui->secondaryArmUpLineEdit->SetDecimalPlaces(1);
    ui->secondaryArmUpLineEdit->setValidator(validator);
    ui->secondayGoBackwardLineEdit->SetDecimalPlaces(1);
    ui->secondayGoBackwardLineEdit->setValidator(validator);
    ui->secondayGoForwardLineEdit->SetDecimalPlaces(1);
    ui->secondayGoForwardLineEdit->setValidator(validator);]

    ui->fixture1GripLineEdit->SetDecimalPlaces(1);
    ui->fixture1GripLineEdit->setValidator(validator);
    ui->fixture1PutOffLineEdit->SetDecimalPlaces(1);
    ui->fixture1PutOffLineEdit->setValidator(validator);*/
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

//    BindingParam_(ui->fixture1GripLineEdit, ICVirtualHost::SM_TIMECLIP1);
//    BindingParam_(ui->fixture2GripLineEdit, ICVirtualHost::SM_TIMECLIP2);
//    BindingParam_(ui->fixture3GripLineEdit, ICVirtualHost::SM_TIMECLIP3);
//    BindingParam_(ui->fixture4GripLineEdit, ICVirtualHost::SM_TIMECLIP4);
//    BindingParam_(ui->sucker1SuckInLineEdit, ICVirtualHost::SM_TIMECLIP5);
//    BindingParam_(ui->sucker2SuckInLineEdit, ICVirtualHost::SM_TIMECLIP6);
//    BindingParam_(ui->injectionCloseLineEdit, ICVirtualHost::SM_TIMECLIP7);
//    BindingParam_(ui->conveyorCloseLineEdit, ICVirtualHost::SM_TIMECLIP8);

//    BindingParam_(ui->fixture1PutOffLineEdit, ICVirtualHost::SM_TIMEPUT1);
//    BindingParam_(ui->fixture2PutOffLineEdit, ICVirtualHost::SM_TIMEPUT2);
//    BindingParam_(ui->fixture3PutOffLineEdit, ICVirtualHost::SM_TIMEPUT3);
//    BindingParam_(ui->fixture4PutOffLineEdit, ICVirtualHost::SM_TIMEPUT4);
//    BindingParam_(ui->sucker1PutOffLineEdit, ICVirtualHost::SM_TIMEPUT5);
//    BindingParam_(ui->sucker2PutOffLineEdit, ICVirtualHost::SM_TIMEPUT6);
//    BindingParam_(ui->injectionOpenLineEdit, ICVirtualHost::SM_TIMEPUT7);
//    BindingParam_(ui->conveyorOpenLineEdit, ICVirtualHost::SM_TIMEPUT8);

    BindingParam_(ui->horizontalLineEdit, ICVirtualHost::SM_TIMEHORI);
    BindingParam_(ui->verticalineEdit, ICVirtualHost::SM_TIMEVERT);
    BindingParam_(ui->secondaryArmDownLineEdit, ICVirtualHost::SM_TIMEVDOWN);
    BindingParam_(ui->secondaryArmUpLineEdit, ICVirtualHost::SM_TIMEVUP);
    BindingParam_(ui->secondayGoForwardLineEdit, ICVirtualHost::SM_TIMEVFOR);
    BindingParam_(ui->secondayGoBackwardLineEdit, ICVirtualHost::SM_TIMEVBACK);
    BindingParam_(ui->mainArmForwardEdit, ICVirtualHost::SM_TIMEMFOR);
    BindingParam_(ui->mainArmBackwardEdit, ICVirtualHost::SM_TIMEMBACK);
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
//    ICMold::CurrentMold()->SaveMoldParamsFile();
    ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
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
