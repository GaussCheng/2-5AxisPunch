#include <QPushButton>
#include "icperipherypage.h"
#include "ui_icperipherypage.h"

#include "icvirtualhost.h"
#include "icvirtualkey.h"
#include "iccommandprocessor.h"
#include "icperipheryparametereditor.h"

ICPeripheryPage::ICPeripheryPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICPeripheryPage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")
{
    ui->setupUi(this);

    QPushButton *buttons = new QPushButton[ui->tableWidget->rowCount()];
    QPushButton * button;
    ICPeripheryParameterEditor* editors = new ICPeripheryParameterEditor[ui->tableWidget->rowCount()];
    ICPeripheryParameterEditor *editor;
    ioNames_<<tr("Injector   ")<<tr("conveyor  ")<<tr("Layout  ")<<tr("Reserve1  ")<<tr("Reserve2  ")
              <<tr("Reserve3  ")<<tr("Reserve4  ")<<tr("Reserve5  ")<<tr("Reserve6  ");
    onClipToOffClip_.insert(ICMold::ACTCLIP7ON, ICMold::ACTCLIP7OFF);
    onClipToOffClip_.insert(ICMold::ACTCLIP8ON, ICMold::ACTCLIP8OFF);
    onClipToOffClip_.insert(ICMold::ACTLAYOUTON, ICMold::ACTLAYOUTOFF);
    onClipToOffClip_.insert(ICMold::ACT_AUX1, ICMold::ACT_AUX1);
    onClipToOffClip_.insert(ICMold::ACT_AUX2, ICMold::ACT_AUX2);
    onClipToOffClip_.insert(ICMold::ACT_AUX3, ICMold::ACT_AUX3);
    onClipToOffClip_.insert(ICMold::ACT_AUX4, ICMold::ACT_AUX4);
    onClipToOffClip_.insert(ICMold::ACT_AUX5, ICMold::ACT_AUX5);
    onClipToOffClip_.insert(ICMold::ACT_AUX6, ICMold::ACT_AUX6);
    offClipToOnClip_.insert(ICMold::ACTCLIP7OFF, ICMold::ACTCLIP7ON);
    offClipToOnClip_.insert(ICMold::ACTCLIP8OFF, ICMold::ACTCLIP8ON);
    offClipToOnClip_.insert(ICMold::ACTLAYOUTOFF, ICMold::ACTLAYOUTON);
    offClipToOnClip_.insert(ICMold::ACT_AUX1, ICMold::ACT_AUX1);
    offClipToOnClip_.insert(ICMold::ACT_AUX2, ICMold::ACT_AUX2);
    offClipToOnClip_.insert(ICMold::ACT_AUX3, ICMold::ACT_AUX3);
    offClipToOnClip_.insert(ICMold::ACT_AUX4, ICMold::ACT_AUX4);
    offClipToOnClip_.insert(ICMold::ACT_AUX5, ICMold::ACT_AUX5);
    offClipToOnClip_.insert(ICMold::ACT_AUX6, ICMold::ACT_AUX6);
    QList<uint> initStatus = onClipToOffClip_.values();
//    QIntValidator *validator = new QIntValidator(0, 2000, this);
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        button = buttons + i;
        button->setIcon(offPixmap_);
        button->setText(ioNames_.at(i));
        editor = editors + i;
//        editor->SetDecimalPlaces(1);
//        editor->setValidator(validator);
//        editor->setText("0.0");
//        delayEdit->setFixedWidth(50);

        settingButtons_.append(button);
        editorVector_.append(editor);
        ui->tableWidget->setCellWidget(i, 1, button);
        ui->tableWidget->setCellWidget(i, 2, editor);

        buttonToClip_.insert(button, initStatus.at(i));
        buttonToLight_.insert(button, 0);
        buttonSignalMapper_.setMapping(button, button);
        connect(button,
                SIGNAL(clicked()),
                &buttonSignalMapper_,
                SLOT(map()));
    }
//    for(int i = 2; i != settingButtons_.size(); ++i)
//    {
//        settingButtons_[i]->hide();
//    }
    ui->tableWidget->resizeColumnsToContents();

    commandKeyMap_.insert(settingButtons_.at(0), qMakePair(static_cast<int>(IC::VKEY_CLIP7ON), static_cast<int>(IC::VKEY_CLIP7OFF)));
    commandKeyMap_.insert(settingButtons_.at(1), qMakePair(static_cast<int>(IC::VKEY_CLIP8ON), static_cast<int>(IC::VKEY_CLIP8OFF)));
    commandKeyMap_.insert(settingButtons_.at(2), qMakePair(static_cast<int>(IC::VKEY_LAYOUTON), static_cast<int>(IC::VKEY_LAYOUTOFF)));
    commandKeyMap_.insert(settingButtons_.at(3), qMakePair(static_cast<int>(IC::VKEY_RESERVE1_ON), static_cast<int>(IC::VKEY_RESERVE1_OFF)));
    commandKeyMap_.insert(settingButtons_.at(4), qMakePair(static_cast<int>(IC::VKEY_RESERVE2_ON), static_cast<int>(IC::VKEY_RESERVE2_OFF)));
    commandKeyMap_.insert(settingButtons_.at(5), qMakePair(static_cast<int>(IC::VKEY_RESERVE3_ON), static_cast<int>(IC::VKEY_RESERVE3_OFF)));
    commandKeyMap_.insert(settingButtons_.at(6), qMakePair(static_cast<int>(IC::VKEY_RESERVE4_ON), static_cast<int>(IC::VKEY_RESERVE4_OFF)));
    commandKeyMap_.insert(settingButtons_.at(7), qMakePair(static_cast<int>(IC::VKEY_RESERVE5_ON), static_cast<int>(IC::VKEY_RESERVE5_OFF)));
    commandKeyMap_.insert(settingButtons_.at(8), qMakePair(static_cast<int>(IC::VKEY_RESERVE6_ON), static_cast<int>(IC::VKEY_RESERVE6_OFF)));

    connect(&buttonSignalMapper_,
            SIGNAL(mapped(QWidget*)),
            this,
            SLOT(CommandButtonClicked(QWidget*)));
}

ICPeripheryPage::~ICPeripheryPage()
{
    delete ui;
}

void ICPeripheryPage::changeEvent(QEvent *e)
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

void ICPeripheryPage::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void ICPeripheryPage::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
    QWidget::hideEvent(e);
}

void ICPeripheryPage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}

void ICPeripheryPage::StatusRefreshed(){}

void ICPeripheryPage::CommandButtonClicked(QWidget *widget)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(widget);
    Q_ASSERT_X(button != NULL, "ICPeripheryPage::CommandButtonClicked", "widget is null");
    int key;
    int currentClip = buttonToClip_.value(button);
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


QList<ICMoldItem> ICPeripheryPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            if(i > 1)
            {
                item.SetIFVal(buttonToLight_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(i, 1))));
            }
            item.SetClip(buttonToClip_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(i, 1))));
            item.SetDVal(editorVector_.at(i)->Delay());
            if(i != 2)
            {
                item.SetSVal(editorVector_.at(i)->Times());
            }
            ret.append(item);
        }
    }
    return ret;
}
