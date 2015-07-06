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
