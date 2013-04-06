#include <QPushButton>
#include "ichcinjectionpage.h"
#include "ui_ichcinjectionpage.h"
#include "icvirtualhost.h"
#include "icactioncommand.h"
#include "icvirtualkey.h"

ICHCInjectionPage::ICHCInjectionPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICHCInjectionPage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")

{
    ui->setupUi(this);

    QPushButton *buttons = new QPushButton[ui->tableWidget->rowCount()];
    QPushButton * button;
    ICLineEditWithVirtualNumericKeypad* delays = new ICLineEditWithVirtualNumericKeypad[ui->tableWidget->rowCount()];
    ICLineEditWithVirtualNumericKeypad *delayEdit;
    ioNames_<<tr("Close Mold Permit 1")
           <<tr("Close Mold Permit 2")
           <<tr("Ejection Permit 1")
          <<tr("Core2 Permit  ")
           <<tr("Ejection Permit 2");
    onClipToOffClip_.insert(ICMold::ACTCLSMDON, ICMold::ACTCLSMDOFF);
    onClipToOffClip_.insert(ICMold::ACT_CORE1_ON, ICMold::ACT_CORE1_OFF);
    onClipToOffClip_.insert(ICMold::ACTEJECTON, ICMold::ACTEJECTOFF);
    onClipToOffClip_.insert(ICMold::ACT_AUX1, ICMold::ACT_AUX1);
//    onClipToOffClip_.insert(ICMold::ACTLAYOUTON, ICMold::ACTLAYOUTOFF);
    onClipToOffClip_.insert(ICMold::ACT_CORE2_ON, ICMold::ACT_CORE2_OFF);

    offClipToOnClip_.insert(ICMold::ACTCLSMDOFF, ICMold::ACTCLSMDON);
    offClipToOnClip_.insert(ICMold::ACT_CORE1_OFF, ICMold::ACT_CORE1_ON);
    offClipToOnClip_.insert(ICMold::ACTEJECTOFF, ICMold::ACTEJECTON);
    offClipToOnClip_.insert(ICMold::ACT_AUX1, ICMold::ACT_AUX1);
//    offClipToOnClip_.insert(ICMold::ACTLAYOUTOFF, ICMold::ACTLAYOUTON);
    offClipToOnClip_.insert(ICMold::ACT_CORE2_OFF, ICMold::ACT_CORE2_ON);
//    QList<uint> initStatus = onClipToOffClip_.values();
    QList<uint> initStatus;
    initStatus.append(ICMold::ACTCLSMDOFF);
    initStatus.append(ICMold::ACT_CORE1_OFF);
    initStatus.append(ICMold::ACTEJECTOFF);
    initStatus.append(ICMold::ACT_CORE2_OFF);
    initStatus.append(ICMold::ACT_AUX1);

    QIntValidator *validator = new QIntValidator(0, 30000, this);
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        button = buttons + i;
        button->setIcon(offPixmap_);
        button->setText(ioNames_.at(i));

        delayEdit = delays + i;
        delayEdit->SetDecimalPlaces(2);
        delayEdit->setValidator(validator);
        delayEdit->setText("0.50");
        //        delayEdit->setFixedWidth(50);

        settingButtons_.append(button);
        delayEdits_.append(delayEdit);
        ui->tableWidget->setCellWidget(i, 1, button);
        ui->tableWidget->setCellWidget(i, 2, delayEdit);

        buttonToClip_.insert(button, initStatus.at(i));
        buttonToLight_.insert(button, 0);
        buttonSignalMapper_.setMapping(button, button);
        connect(button,
                SIGNAL(clicked()),
                &buttonSignalMapper_,
                SLOT(map()));
    }
    ui->tableWidget->setColumnWidth(0, 50);
    ui->tableWidget->setColumnWidth(1, 140);


    commandKeyMap_.insert(settingButtons_.at(0), qMakePair(static_cast<int>(IC::VKEY_CLSMDON), static_cast<int>(IC::VKEY_CLSMDOFF)));
    commandKeyMap_.insert(settingButtons_.at(1), qMakePair(0, 0));
    commandKeyMap_.insert(settingButtons_.at(2), qMakePair(static_cast<int>(IC::VKEY_EJECTON), static_cast<int>(IC::VKEY_EJECTOFF)));
    commandKeyMap_.insert(settingButtons_.at(3), qMakePair(static_cast<int>(IC::VKEY_RESERVE1_ON), static_cast<int>(IC::VKEY_RESERVE1_OFF)));
    commandKeyMap_.insert(settingButtons_.at(4), qMakePair(0, 0));
//    commandKeyMap_.insert(settingButtons_.at(0), qMakePair(static_cast<int>(IC::VKEY_CLSMDON), static_cast<int>(IC::VKEY_CLSMDOFF)));
//    commandKeyMap_.insert(settingButtons_.at(1), qMakePair(static_cast<int>(IC::VKEY_EJECTON), static_cast<int>(IC::VKEY_EJECTOFF)));
//    commandKeyMap_.insert(settingButtons_.at(2), qMakePair(0, 0));
//    commandKeyMap_.insert(settingButtons_.at(3), qMakePair(0, 0));

    connect(&buttonSignalMapper_,
            SIGNAL(mapped(QWidget*)),
            this,
            SLOT(CommandButtonClicked(QWidget*)));
}

ICHCInjectionPage::~ICHCInjectionPage()
{
    delete ui;
}

void ICHCInjectionPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ioNames_.clear();
        ioNames_<<tr("Close Mold Permit 1")
               <<tr("Close Mold Permit 2")
               <<tr("Ejection Permit 1")
               <<tr("Core2 Permit  ")
               <<tr("Ejection Permit 2");
        for(int i = 0; i != settingButtons_.size(); ++i)
        {
            settingButtons_[i]->setText(ioNames_.at(i));
        }
        break;
    default:
        break;
    }
}

void ICHCInjectionPage::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
}

void ICHCInjectionPage::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
}

void ICHCInjectionPage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}

void ICHCInjectionPage::StatusRefreshed()
{
    //    static ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    //    QMap<QLabel*, int>::iterator p = currentPosMap_.begin();
    //    while(p != currentPosMap_.end())
    //    {
    //        if(host->IsClipOn(p.value()))
    //        {
    //            if(!ledIsOnMap_.value(p.key()))
    //            {
    //                p.key()->setPixmap(onPixmap_);
    //                ledIsOnMap_.insert(p.key(), true);
    //            }
    //        }
    //        else
    //        {
    //            if(ledIsOnMap_.value(p.key()))
    //            {
    //                p.key()->setPixmap(offPixmap_);
    //                ledIsOnMap_.insert(p.key(), false);
    //            }
    //        }
    //        ++p;
    //    }
}

void ICHCInjectionPage::CommandButtonClicked(QWidget *widget)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(widget);
    Q_ASSERT_X(button != NULL, "ICHCFixturePage::CommandButtonClicked", "widget is null");
    int key;
    int currentClip = buttonToClip_.value(button);
//    if(onClipToOffClip_.contains(currentClip))
//    {
//        key = commandKeyMap_.value(button).second;
//        buttonToClip_.insert(button, onClipToOffClip_.value(currentClip));
//        button->setIcon(offPixmap_);
//    }
//    else
//    {
//        key = commandKeyMap_.value(button).first;
//        buttonToClip_.insert(button, offClipToOnClip_.value(currentClip));
//        button->setIcon(onPixmap_);
//    }

    if(buttonToLight_.value(button) == 1)
    {
        key = commandKeyMap_.value(button).second;
        buttonToClip_.insert(button, onClipToOffClip_.value(currentClip));
        button->setIcon(offPixmap_);
        buttonToLight_.insert(button, 0);
    }
    else
    {
        key = commandKeyMap_.value(button).first;
        buttonToClip_.insert(button, offClipToOnClip_.value(currentClip));
        button->setIcon(onPixmap_);
        buttonToLight_.insert(button, 1);
    }

    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(key);

}

QList<ICMoldItem> ICHCInjectionPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;

    ICMoldItem item;
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(i == 4)
        {
            continue ;
        }
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetClip(buttonToClip_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(i, 1))));
            item.SetDVal(delayEdits_.at(i)->TransThisTextToThisInt());
            ret.append(item);
        }
    }
    if(ui->tableWidget->item(4,0)->checkState() == Qt::Checked)
    {
        item.SetIFVal(buttonToLight_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(4, 1))));
        item.SetClip(buttonToClip_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(4, 1))));
        item.SetDVal(delayEdits_.at(4)->TransThisTextToThisInt());
        ret.append(item);
    }
    return ret;
}
