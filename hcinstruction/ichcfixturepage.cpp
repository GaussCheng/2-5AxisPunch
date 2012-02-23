#include <QLabel>
#include <QPushButton>

#include "ichcfixturepage.h"
#include "ui_ichcfixturepage.h"

#include "icvirtualhost.h"
#include "icvirtualkey.h"
#include "iccommandprocessor.h"


ICHCFixturePage::ICHCFixturePage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICHCFixturePage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")
{
    ui->setupUi(this);

    QPushButton *buttons = new QPushButton[ui->tableWidget->rowCount()];
    QPushButton * button;
    ICLineEditWithVirtualNumericKeypad* delays = new ICLineEditWithVirtualNumericKeypad[ui->tableWidget->rowCount()];
    ICLineEditWithVirtualNumericKeypad *delayEdit;
    ioNames_<<tr("Fixture1  ")<<tr("Fixture2  ")<<tr("Fixture3  ")<<tr("Fixture4  ")
               <<tr("Sucker1   ")<<tr("Sucker2   ");
    onClipToOffClip_.insert(ICMold::ACTCLIP1ON, ICMold::ACTCLIP1OFF);
    onClipToOffClip_.insert(ICMold::ACTCLIP2ON, ICMold::ACTCLIP2OFF);
    onClipToOffClip_.insert(ICMold::ACTCLIP3ON, ICMold::ACTCLIP3OFF);
    onClipToOffClip_.insert(ICMold::ACTCLIP4ON, ICMold::ACTCLIP4OFF);
    onClipToOffClip_.insert(ICMold::ACTCLIP5ON, ICMold::ACTCLIP5OFF);
    onClipToOffClip_.insert(ICMold::ACTCLIP6ON, ICMold::ACTCLIP6OFF);
    offClipToOnClip_.insert(ICMold::ACTCLIP1OFF, ICMold::ACTCLIP1ON);
    offClipToOnClip_.insert(ICMold::ACTCLIP2OFF, ICMold::ACTCLIP2ON);
    offClipToOnClip_.insert(ICMold::ACTCLIP3OFF, ICMold::ACTCLIP3ON);
    offClipToOnClip_.insert(ICMold::ACTCLIP4OFF, ICMold::ACTCLIP4ON);
    offClipToOnClip_.insert(ICMold::ACTCLIP5OFF, ICMold::ACTCLIP5ON);
    offClipToOnClip_.insert(ICMold::ACTCLIP6OFF, ICMold::ACTCLIP6ON);
    QList<uint> initStatus = onClipToOffClip_.values();
    QIntValidator *validator = new QIntValidator(0, 2000, this);
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        button = buttons + i;
        button->setIcon(offPixmap_);
        button->setText(ioNames_.at(i));

        delayEdit = delays + i;
        delayEdit->SetDecimalPlaces(2);
        delayEdit->setValidator(validator);
        delayEdit->setText("0.00");
//        delayEdit->setFixedWidth(50);

        settingButtons_.append(button);
        delayEdits_.append(delayEdit);
        ui->tableWidget->setCellWidget(i, 1, button);
        ui->tableWidget->setCellWidget(i, 2, delayEdit);

        buttonToClip_.insert(button, initStatus.at(i));
        buttonSignalMapper_.setMapping(button, button);
        connect(button,
                SIGNAL(clicked()),
                &buttonSignalMapper_,
                SLOT(map()));
    }
    ui->tableWidget->resizeColumnsToContents();

//    currentPosMap_.insert(ui->fixture1CurrentLabel, 0);
//    currentPosMap_.insert(ui->fixture2CurrentLabel, 1);
//    currentPosMap_.insert(ui->fixture3CurrentLabel, 2);
//    currentPosMap_.insert(ui->fixture4CurrentLabel, 3);
//    currentPosMap_.insert(ui->sucker1CurrentLabel, 4);
//    currentPosMap_.insert(ui->sucker2CurrentLabel, 5);
//    currentPosMap_.insert(ui->injectionCurrentLabel, 6);
//    currentPosMap_.insert(ui->conveyorCurrentLabel, 7);

    commandKeyMap_.insert(settingButtons_.at(0), qMakePair(static_cast<int>(IC::VKEY_CLIP1ON), static_cast<int>(IC::VKEY_CLIP1OFF)));
    commandKeyMap_.insert(settingButtons_.at(1), qMakePair(static_cast<int>(IC::VKEY_CLIP2ON), static_cast<int>(IC::VKEY_CLIP2OFF)));
    commandKeyMap_.insert(settingButtons_.at(2), qMakePair(static_cast<int>(IC::VKEY_CLIP3ON), static_cast<int>(IC::VKEY_CLIP3OFF)));
    commandKeyMap_.insert(settingButtons_.at(3), qMakePair(static_cast<int>(IC::VKEY_CLIP4ON), static_cast<int>(IC::VKEY_CLIP4OFF)));
    commandKeyMap_.insert(settingButtons_.at(4), qMakePair(static_cast<int>(IC::VKEY_CLIP5ON), static_cast<int>(IC::VKEY_CLIP5OFF)));
    commandKeyMap_.insert(settingButtons_.at(5), qMakePair(static_cast<int>(IC::VKEY_CLIP6ON), static_cast<int>(IC::VKEY_CLIP6OFF)));

    connect(&buttonSignalMapper_,
            SIGNAL(mapped(QWidget*)),
            this,
            SLOT(CommandButtonClicked(QWidget*)));
//    QList<QAbstractButton*> buttons;
//    QList<QLabel*> labels;
//    buttons<<ui->fixture1Button<<ui->fixture2Button
//           <<ui->fixture3Button<<ui->fixture4Button
//           <<ui->suck1Button<<ui->suck2Button
//           <<ui->injectorButton<<ui->conveyorButton;
//    labels<<ui->fixture1CurrentLabel<<ui->fixture2CurrentLabel
//          <<ui->fixture3CurrentLabel<<ui->fixture4CurrentLabel
//          <<ui->sucker1CurrentLabel<<ui->sucker2CurrentLabel
//          <<ui->injectionCurrentLabel<<ui->conveyorCurrentLabel;
//    for(int i = 0; i != buttons.size(); ++i)
//    {
//        connect(buttons.at(i),
//                SIGNAL(clicked()),
//                &buttonSignalMapper_,
//                SLOT(map()));
//        buttonSignalMapper_.setMapping(buttons.at(i), buttons.at(i));
//        ledIsOnMap_.insert(labels.at(i), false);
//        commandButtonToLabel_.insert(buttons.at(i), labels.at(i));
//    }

//    QList<ICLineEditWithVirtualNumericKeypad*> delayEditors;
//    delayEditors<<ui->fixture1DelayEdit<<ui->fixture2DelayEdit
//                <<ui->fixture3DelayEdit<<ui->fixture4DelayEdit
//                <<ui->sucker1DelayEdit<<ui->sucker2DelayEdit
//                <<ui->injectionDelayEdit<<ui->conveyorDelayEdit;
//    QIntValidator * validator = new QIntValidator(0, 32767, this);
//    for(int i = 0; i != delayEditors.size(); ++i)
//    {
//        delayEditors[i]->SetDecimalPlaces(1);
//        delayEditors[i]->setValidator(validator);
//    }
//    ui->conveyorOtherEdit->setValidator(validator);
//    ui->injectionOtherEdit->setValidator(validator);

}

ICHCFixturePage::~ICHCFixturePage()
{
    delete ui;
}

void ICHCFixturePage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICHCFixturePage::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void ICHCFixturePage::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
    QWidget::hideEvent(e);
}

void ICHCFixturePage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}


void ICHCFixturePage::StatusRefreshed()
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

void ICHCFixturePage::CommandButtonClicked(QWidget *widget)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(widget);
    Q_ASSERT_X(button != NULL, "ICHCFixturePage::CommandButtonClicked", "widget is null");
    int key;
    int currentClip = buttonToClip_.value(button);
    if(onClipToOffClip_.contains(currentClip))
    {
        key = commandKeyMap_.value(button).second;
        buttonToClip_.insert(button, onClipToOffClip_.value(currentClip));
        button->setIcon(offPixmap_);
    }
    else
    {
        key = commandKeyMap_.value(button).first;
        buttonToClip_.insert(button, offClipToOnClip_.value(currentClip));
        button->setIcon(onPixmap_);
    }
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(key);
}


QList<ICMoldItem> ICHCFixturePage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetClip(buttonToClip_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(i, 1))));
            item.SetDVal(delayEdits_.at(i)->TransThisTextToThisInt());
            ret.append(item);
        }
    }
    return ret;
}
