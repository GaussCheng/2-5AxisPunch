#include <QPushButton>
#include "iccutpage.h"
#include "ui_iccutpage.h"

#include "icvirtualhost.h"
#include "icactioncommand.h"
#include "icvirtualkey.h"

ICCutPage::ICCutPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICCutPage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")
{
    ui->setupUi(this);
    QPushButton *buttons = new QPushButton[ui->tableWidget->rowCount()];
    QPushButton * button;
//    ICLineEditWithVirtualNumericKeypad* delays = new ICLineEditWithVirtualNumericKeypad[ui->tableWidget->rowCount()];
//    ICLineEditWithVirtualNumericKeypad *delayEdit;
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
//    QIntValidator *validator = new QIntValidator(0, 2000, this);
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        button = buttons + i;
        button->setIcon(offPixmap_);
        button->setText(ioNames_.at(i));

//        delayEdit = delays + i;
//        delayEdit->SetDecimalPlaces(1);
//        delayEdit->setValidator(validator);
//        delayEdit->setText("0.0");

        settingButtons_.append(button);
//        delayEdits_.append(delayEdit);
        ui->tableWidget->setCellWidget(i, 1, button);
//        ui->tableWidget->setCellWidget(i, 2, delayEdit);

        buttonToClip_.insert(button, initStatus.at(i));
        buttonSignalMapper_.setMapping(button, button);
        connect(button,
                SIGNAL(clicked()),
                &buttonSignalMapper_,
                SLOT(map()));
    }
    ui->tableWidget->resizeColumnsToContents();

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
}

ICCutPage::~ICCutPage()
{
    delete ui;
}
void ICCutPage::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void ICCutPage::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
    QWidget::hideEvent(e);
}

void ICCutPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ioNames_.clear();
        ioNames_<<tr("Fixture1  ")<<tr("Fixture2  ")<<tr("Fixture3  ")<<tr("Fixture4  ")
                   <<tr("Sucker1   ")<<tr("Sucker2   ");
        for(int i = 0; i != settingButtons_.size(); ++i)
        {
            settingButtons_[i]->setText(ioNames_.at(i));
        }
        break;
    default:
        break;
    }
}

void ICCutPage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}


void ICCutPage::StatusRefreshed()
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

void ICCutPage::CommandButtonClicked(QWidget *widget)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(widget);
    Q_ASSERT_X(button != NULL, "ICCutPage::CommandButtonClicked", "widget is null");
    int currentClip = buttonToClip_.value(button);
    if(onClipToOffClip_.contains(currentClip))
    {
//        int key = commandKeyMap_.value(button).second;
        buttonToClip_.insert(button, onClipToOffClip_.value(currentClip));
        button->setIcon(offPixmap_);
    }
    else
    {
//        key = commandKeyMap_.value(button).first;
        buttonToClip_.insert(button, offClipToOnClip_.value(currentClip));
        button->setIcon(onPixmap_);
    }
//    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(key);
}


QList<ICMoldItem> ICCutPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    uint val;
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            val = buttonToClip_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(i, 1)));
            if(onClipToOffClip_.contains(val))
            {
                item.SetIFVal(1);
            }
            else
            {
                item.SetIFVal(0);
            }
            item.SetSVal(i);
            item.SetAction(ICMold::ACT_Cut);
//            item.SetIFVal(buttonToClip_.value())
//            item.SetDVal(delayEdits_.at(i)->TransThisTextToThisInt());
            ret.append(item);
        }
    }
    return ret;
}
