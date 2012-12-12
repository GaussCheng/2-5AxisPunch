#include "ichcdetectionframe.h"
#include "ui_ichcdetectionframe.h"

#include "ictwoselectioncomboboxwrapper.h"
#include "icmold.h"
#include "icvirtualhost.h"
//#include "icactioncommand.h"

ICHCDetectionFrame::ICHCDetectionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCDetectionFrame)
{
    ui->setupUi(this);

    ICTwoSelectionComboBoxWrapper* wrapper;
    wrapper = new ICTwoSelectionComboBoxWrapper(ui->detectFixture1ComboBox, ICMold::CheckClip1);
    wrappers_.append(wrapper);
    wrapper = new ICTwoSelectionComboBoxWrapper(ui->detectFixture2ComboBox, ICMold::CheckClip2);
    wrappers_.append(wrapper);
    wrapper = new ICTwoSelectionComboBoxWrapper(ui->detectFixture3ComboBox, ICMold::CheckClip3);
    wrappers_.append(wrapper);
    wrapper = new ICTwoSelectionComboBoxWrapper(ui->detectFixture4ComboBox, ICMold::CheckClip4);
    wrappers_.append(wrapper);
    wrapper = new ICTwoSelectionComboBoxWrapper(ui->detectSucker1ComboBox, ICMold::CheckClip5);
    wrappers_.append(wrapper);
    wrapper = new ICTwoSelectionComboBoxWrapper(ui->detectSucker2ComboBox, ICMold::CheckClip6);
    wrappers_.append(wrapper);
    //    wrapper = new ICTwoSelectionComboBoxWrapper(ui->converyotComboBox, ICMold::CheckClip7);
    //    wrappers_.append(wrapper);
    //    wrapper = new ICTwoSelectionComboBoxWrapper(ui->injectionComboBox, ICMold::CheckClip8);
    //    wrappers_.append(wrapper);
    //    ui->converyotComboBox->hide();
    //    ui->injectionComboBox->hide();
    //    ui->label_13->hide();
    //    ui->label_8->hide();
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ui->detectPressureComboBox->setCurrentIndex(qAbs(host->IsPressureCheck() - 1));
    ui->detectSecurityComboBox->setCurrentIndex(qAbs(host->IsSecurityCheck() - 1));
    ui->detectMidMoldComboBox->setCurrentIndex(qAbs(host->IsMidMoldCheck() - 1));
    ui->ejectionLinkLockBox->setCurrentIndex(qAbs(host->IsEjectionLink() - 1));
    ui->detectOriginBox->setCurrentIndex(qAbs(host->IsAlarmWhenOrigin() - 1));
    //    ui->detectPositionBox->setCurrentIndex(qAbs(host->IsPositionDetect() - 1));
    ui->detectPositionBox->setCurrentIndex(host->TranserferPosition());
    ui->originPositionBox->setCurrentIndex(host->OriginPosition());


    connect(ICMold::CurrentMold(),
            SIGNAL(MoldNumberParamChanged()),
            this,
            SLOT(OnMoldNumberParamChanged()));
}

ICHCDetectionFrame::~ICHCDetectionFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void ICHCDetectionFrame::hideEvent(QHideEvent *e)
{
    if(ICVirtualHost::GlobalVirtualHost()->IsParamChanged())
    {
        qDebug("Detection hide");
        ICMold::CurrentMold()->SaveMoldParamsFile();
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    //    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
    QFrame::hideEvent(e);
}

void ICHCDetectionFrame::changeEvent(QEvent *e)
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
void ICHCDetectionFrame::on_detectPressureComboBox_activated(int index)
{
    bool isCheck = qAbs(index - 1);
    ICVirtualHost::GlobalVirtualHost()->SetPressureCheck(isCheck);
}

void ICHCDetectionFrame::on_detectSecurityComboBox_activated(int index)
{
    bool isCheck = qAbs(index - 1);
    ICVirtualHost::GlobalVirtualHost()->SetSecurityCheck(isCheck);
}

void ICHCDetectionFrame::on_detectMidMoldComboBox_activated(int index)
{
    bool isCheck = qAbs(index - 1);
    ICVirtualHost::GlobalVirtualHost()->SetMidMoldCheck(isCheck);
}

void ICHCDetectionFrame::OnMoldNumberParamChanged()
{
    for(int i = 0; i != wrappers_.size(); ++i)
    {
        wrappers_[i]->UpdateParameter();
    }
}

//void ICHCDetectionFrame::on_closeMoldStatusComboBox_activated(int index)
//{
//    bool isCheck = qAbs(index - 1);
//    ICVirtualHost::GlobalVirtualHost()->SetCloseMoldPermit(isCheck);
//}

void ICHCDetectionFrame::on_ejectionLinkLockBox_activated(int index)
{
    bool isCheck = qAbs(index - 1);
    ICVirtualHost::GlobalVirtualHost()->SetEjectionLink(isCheck);
}

void ICHCDetectionFrame::on_detectPositionBox_activated(int index)
{
    //    bool isCheck = qAbs(index - 1);
    //    ICVirtualHost::GlobalVirtualHost()->SetPositionDetect(isCheck);
    ICVirtualHost::GlobalVirtualHost()->SetTranserferPosition(index);
}

void ICHCDetectionFrame::on_detectOriginBox_activated(int index)
{
    bool isCheck = qAbs(index - 1);
    ICVirtualHost::GlobalVirtualHost()->SetAlarmWhenOrigin(isCheck);
}

void ICHCDetectionFrame::on_originPositionBox_activated(int index)
{
    ICVirtualHost::GlobalVirtualHost()->SetOriginPosition(index);
}

void ICHCDetectionFrame::RetranslateUi_()
{
//    label->setText(QApplication::translate("ICHCDetectionFrame", "Detect fixture 1", 0, QApplication::UnicodeUTF8));
//    ui->label->setText(tr("Detect fixture 1"))
//    detectFixture1ComboBox->clear();
//    detectFixture1ComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Reversed Phase", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Positive Phase", 0, QApplication::UnicodeUTF8)
//    );
//    label_5->setText(QApplication::translate("ICHCDetectionFrame", "Detect sucker 1", 0, QApplication::UnicodeUTF8));
//    detectSucker1ComboBox->clear();
//    detectSucker1ComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Reversed Phase", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Positive Phase", 0, QApplication::UnicodeUTF8)
//    );
//    label_2->setText(QApplication::translate("ICHCDetectionFrame", "Detect fixture 2", 0, QApplication::UnicodeUTF8));
//    detectFixture2ComboBox->clear();
//    detectFixture2ComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Reversed Phase", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Positive Phase", 0, QApplication::UnicodeUTF8)
//    );
//    label_6->setText(QApplication::translate("ICHCDetectionFrame", "Detect sucker 2", 0, QApplication::UnicodeUTF8));
//    detectSucker2ComboBox->clear();
//    detectSucker2ComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Reversed Phase", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Positive Phase", 0, QApplication::UnicodeUTF8)
//    );
//    label_3->setText(QApplication::translate("ICHCDetectionFrame", "Detect fixture 3", 0, QApplication::UnicodeUTF8));
//    detectFixture3ComboBox->clear();
//    detectFixture3ComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Reversed Phase", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Positive Phase", 0, QApplication::UnicodeUTF8)
//    );
//    label_4->setText(QApplication::translate("ICHCDetectionFrame", "Detect fixture 4", 0, QApplication::UnicodeUTF8));
//    detectFixture4ComboBox->clear();
//    detectFixture4ComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Reversed Phase", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Positive Phase", 0, QApplication::UnicodeUTF8)
//    );
//    label_11->setText(QApplication::translate("ICHCDetectionFrame", "Detect Mid Mold", 0, QApplication::UnicodeUTF8));
//    detectMidMoldComboBox->clear();
//    detectMidMoldComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Use", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Not Use", 0, QApplication::UnicodeUTF8)
//    );
//    label_10->setText(QApplication::translate("ICHCDetectionFrame", "Detect Security Door", 0, QApplication::UnicodeUTF8));
//    label_9->setText(QApplication::translate("ICHCDetectionFrame", "Detect Pressure", 0, QApplication::UnicodeUTF8));
//    detectPressureComboBox->clear();
//    detectPressureComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Not Use", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Use", 0, QApplication::UnicodeUTF8)
//    );
//    detectSecurityComboBox->clear();
//    detectSecurityComboBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Not Use", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Use", 0, QApplication::UnicodeUTF8)
//    );
//    label_12->setText(QApplication::translate("ICHCDetectionFrame", "Ejection Link Lock", 0, QApplication::UnicodeUTF8));
//    ejectionLinkLockBox->clear();
//    ejectionLinkLockBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Not Use", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Use", 0, QApplication::UnicodeUTF8)
//    );
//    label_13->setText(QApplication::translate("ICHCDetectionFrame", "Detect Postion", 0, QApplication::UnicodeUTF8));
//    detectOriginBox->clear();
//    detectOriginBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Need Mold Opened Signal", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Don't need Mold Opened Signal", 0, QApplication::UnicodeUTF8)
//    );
//    label_14->setText(QApplication::translate("ICHCDetectionFrame", "Detect Origin", 0, QApplication::UnicodeUTF8));
//    detectPositionBox->clear();
//    detectPositionBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Vertical", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Horizontal", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Don't Limit", 0, QApplication::UnicodeUTF8)
//    );
//    label_15->setText(QApplication::translate("ICHCDetectionFrame", "Orgin Position", 0, QApplication::UnicodeUTF8));
//    originPositionBox->clear();
//    originPositionBox->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Vertical", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Horizontal", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "No Limit", 0, QApplication::UnicodeUTF8)
//    );
//    label_7->setText(QApplication::translate("ICHCDetectionFrame", "IMM Emergency", 0, QApplication::UnicodeUTF8));
//    detectSecurityComboBox_2->clear();
//    detectSecurityComboBox_2->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Not Use", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Use", 0, QApplication::UnicodeUTF8)
//    );
//    label_16->setText(QApplication::translate("ICHCDetectionFrame", "Down Position", 0, QApplication::UnicodeUTF8));
//    originPositionBox_2->clear();
//    originPositionBox_2->insertItems(0, QStringList()
//     << QApplication::translate("ICHCDetectionFrame", "Vertical", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "Horizontal", 0, QApplication::UnicodeUTF8)
//     << QApplication::translate("ICHCDetectionFrame", "No Limit", 0, QApplication::UnicodeUTF8)
//    );
}
