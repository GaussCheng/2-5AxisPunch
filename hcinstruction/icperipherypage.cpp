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
//    ui->lyDelayLineEdit->SetDecimalPlaces(2);
//    ui->groupLineEdit->setValidator(new QIntValidator(0, 3, this));
//    ui->lyDelayLineEdit->setValidator(new QIntValidator(0, 32767, this));

    QPushButton *buttons = new QPushButton[ui->actionWidget->rowCount()];
    QPushButton * button;
    ICPeripheryParameterEditor* editors = new ICPeripheryParameterEditor[ui->actionWidget->rowCount()];
    ICPeripheryParameterEditor *editor;
    ioNames_<<tr("Injector   ")<<tr("conveyor  ");
    onClipToOffClip_.insert(ICMold::ACTCLIP7ON, ICMold::ACTCLIP7OFF);
    onClipToOffClip_.insert(ICMold::ACTCLIP8ON, ICMold::ACTCLIP8OFF);
    offClipToOnClip_.insert(ICMold::ACTCLIP7OFF, ICMold::ACTCLIP7ON);
    offClipToOnClip_.insert(ICMold::ACTCLIP8OFF, ICMold::ACTCLIP8ON);

    QList<uint> initStatus = onClipToOffClip_.values();
    //    QIntValidator *validator = new QIntValidator(0, 2000, this);
    for(int i = 0; i != ui->actionWidget->rowCount(); ++i)
    {
        button = buttons + i;
        button->setIcon(offPixmap_);
        button->setText(ioNames_.at(i));
        editor = editors + i;
        settingButtons_.append(button);
        editorVector_.append(editor);
        ui->actionWidget->setCellWidget(i, 1, button);
        ui->actionWidget->setCellWidget(i, 2, editor);

        buttonToClip_.insert(button, initStatus.at(i));
        buttonToLight_.insert(button, 0);
        buttonSignalMapper_.setMapping(button, button);
        connect(button,
                SIGNAL(clicked()),
                &buttonSignalMapper_,
                SLOT(map()));

    }
    ui->actionWidget->setColumnWidth(0, 50);
    ui->actionWidget->setColumnWidth(1, 110);

    commandKeyMap_.insert(settingButtons_.at(0), qMakePair(static_cast<int>(IC::VKEY_CLIP7ON), static_cast<int>(IC::VKEY_CLIP7OFF)));
    commandKeyMap_.insert(settingButtons_.at(1), qMakePair(static_cast<int>(IC::VKEY_CLIP8ON), static_cast<int>(IC::VKEY_CLIP8OFF)));

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
//    connect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(StatusRefreshed()),
//            this,
//            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void ICPeripheryPage::hideEvent(QHideEvent *e)
{
//    disconnect(ICVirtualHost::GlobalVirtualHost(),
//               SIGNAL(StatusRefreshed()),
//               this,
//               SLOT(StatusRefreshed()));
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
    for(int i = 0; i != ui->actionWidget->rowCount(); ++i)
    {
        if(ui->actionWidget->item(i, 0)->checkState() == Qt::Checked)
        {
            item.SetClip(buttonToClip_.value(qobject_cast<QAbstractButton*>(ui->actionWidget->cellWidget(i, 1))));
            item.SetDVal(editorVector_.at(i)->Delay());
            item.SetSVal(editorVector_.at(i)->Times());
            ret.append(item);
        }
    }
    return ret;
}
