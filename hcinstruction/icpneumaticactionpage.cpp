#include <QPushButton>
#include "icpneumaticactionpage.h"
#include "ui_icpneumaticactionpage.h"
#include "icvirtualhost.h"
#include "icvirtualkey.h"
#include "iccommandprocessor.h"
#include "icparameterssave.h"

#include "icperipheryparametereditor.h"
ICPneumaticActionPage::ICPneumaticActionPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICPneumaticActionPage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")
{
    ui->setupUi(this);
    QPushButton *buttons = new QPushButton[ui->tableWidget->rowCount()];
    QPushButton * button;
    ICPeripheryParameterEditor* editors = new ICPeripheryParameterEditor[ui->tableWidget->rowCount()];
    ICPeripheryParameterEditor *editor;
    ioNames_<<tr("Reserve1  ")<<tr("Reserve2  ")
           <<tr("Reserve3  ")<<tr("Reserve4  ")<<tr("Reserve5  ")<<tr("Reserve6  ");
    onClipToOffClip_.insert(ICMold::ACT_AUX1, ICMold::ACT_AUX1);
    onClipToOffClip_.insert(ICMold::ACT_AUX2, ICMold::ACT_AUX2);
    onClipToOffClip_.insert(ICMold::ACT_AUX3, ICMold::ACT_AUX3);
    onClipToOffClip_.insert(ICMold::ACT_AUX4, ICMold::ACT_AUX4);
    onClipToOffClip_.insert(ICMold::ACT_AUX5, ICMold::ACT_AUX5);
    onClipToOffClip_.insert(ICMold::ACT_AUX6, ICMold::ACT_AUX6);

    offClipToOnClip_.insert(ICMold::ACT_AUX1, ICMold::ACT_AUX1);
    offClipToOnClip_.insert(ICMold::ACT_AUX2, ICMold::ACT_AUX2);
    offClipToOnClip_.insert(ICMold::ACT_AUX3, ICMold::ACT_AUX3);
    offClipToOnClip_.insert(ICMold::ACT_AUX4, ICMold::ACT_AUX4);
    offClipToOnClip_.insert(ICMold::ACT_AUX5, ICMold::ACT_AUX5);
    offClipToOnClip_.insert(ICMold::ACT_AUX6, ICMold::ACT_AUX6);
    QList<uint> initStatus = onClipToOffClip_.values();
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        button = buttons + i;
        button->setIcon(offPixmap_);
        button->setText(ioNames_.at(i));
        editor = editors + i;
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

    ui->tableWidget->setColumnWidth(0, 50);
    ui->tableWidget->setColumnWidth(1, 140);

    commandKeyMap_.insert(settingButtons_.at(0), qMakePair(static_cast<int>(IC::VKEY_RESERVE1_ON), static_cast<int>(IC::VKEY_RESERVE1_OFF)));
    commandKeyMap_.insert(settingButtons_.at(1), qMakePair(static_cast<int>(IC::VKEY_RESERVE2_ON), static_cast<int>(IC::VKEY_RESERVE2_OFF)));
    commandKeyMap_.insert(settingButtons_.at(2), qMakePair(static_cast<int>(IC::VKEY_RESERVE3_ON), static_cast<int>(IC::VKEY_RESERVE3_OFF)));
    commandKeyMap_.insert(settingButtons_.at(3), qMakePair(static_cast<int>(IC::VKEY_RESERVE4_ON), static_cast<int>(IC::VKEY_RESERVE4_OFF)));
    commandKeyMap_.insert(settingButtons_.at(4), qMakePair(static_cast<int>(IC::VKEY_RESERVE5_ON), static_cast<int>(IC::VKEY_RESERVE5_OFF)));
    commandKeyMap_.insert(settingButtons_.at(5), qMakePair(static_cast<int>(IC::VKEY_RESERVE6_ON), static_cast<int>(IC::VKEY_RESERVE6_OFF)));

    connect(&buttonSignalMapper_,
            SIGNAL(mapped(QWidget*)),
            this,
            SLOT(CommandButtonClicked(QWidget*)));
}

ICPneumaticActionPage::~ICPneumaticActionPage()
{
    delete ui;
}

void ICPneumaticActionPage::changeEvent(QEvent *e)
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

void ICPneumaticActionPage::showEvent(QShowEvent *e)
{
//    if(ICParametersSave::Instance()->IsSingleArm())
//    {
////        ui->subArmGroupBox->hide();
//    }
//    else
//    {
////        ui->subArmGroupBox->show();
//    }
    ICInstructionEditorBase::showEvent(e);
}

void ICPneumaticActionPage::CommandButtonClicked(QWidget *widget)
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


QList<ICMoldItem> ICPneumaticActionPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetIFVal(buttonToLight_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(i, 1))));
            item.SetClip(buttonToClip_.value(qobject_cast<QAbstractButton*>(ui->tableWidget->cellWidget(i, 1))));
            item.SetDVal(editorVector_.at(i)->Delay());
            item.SetSVal(editorVector_.at(i)->Times());
            ret.append(item);
        }
    }
    return ret;
}
