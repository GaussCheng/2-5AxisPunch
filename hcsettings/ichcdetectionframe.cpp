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
        RetranslateUi_();
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
    const QString rp = tr("Reversed Phase");
    const QString pp = tr("Positive Phase");
    const QString use = tr("Use");
    const QString noUse = tr("Not Use");
    ui->label->setText(tr("Detect fixture 1"));
    ui->detectFixture1ComboBox->setItemText(0, rp);
    ui->detectFixture1ComboBox->setItemText(1, pp);
    ui->label_5->setText(tr("Detect sucker 1"));
    ui->detectSucker1ComboBox->setItemText(0, rp);
    ui->detectSucker1ComboBox->setItemText(1, pp);
    ui->label_2->setText(tr("Detect fixture 2"));
    ui->detectFixture2ComboBox->setItemText(0, rp);
    ui->detectFixture2ComboBox->setItemText(1, pp);
    ui->label_6->setText(tr("Detect sucker 2"));
    ui->detectSucker2ComboBox->setItemText(0, rp);
    ui->detectSucker2ComboBox->setItemText(1, pp);
    ui->label_3->setText(tr("Detect fixture 3"));
    ui->detectFixture3ComboBox->setItemText(0, rp);
    ui->detectFixture3ComboBox->setItemText(1, pp);
    ui->label_4->setText(tr("Detect fixture 4"));
    ui->detectFixture4ComboBox->setItemText(0, rp);
    ui->detectFixture4ComboBox->setItemText(1, pp);
    ui->label_11->setText(tr("Detect Mid Mold"));
    ui->detectMidMoldComboBox->setItemText(0, use);
    ui->detectMidMoldComboBox->setItemText(1, noUse);
    ui->label_10->setText(tr("Detect Security Door"));
    ui->label_9->setText(tr("Detect Pressure"));
    ui->detectPressureComboBox->setItemText(0, noUse);
    ui->detectPressureComboBox->setItemText(1, use);
    ui->detectSecurityComboBox->setItemText(0, noUse);
    ui->detectSecurityComboBox->setItemText(1, use);
    ui->label_12->setText(tr("Ejection Link Lock"));
    ui->ejectionLinkLockBox->setItemText(0, noUse);
    ui->ejectionLinkLockBox->setItemText(1, use);
    ui->label_13->setText(tr("Detect Postion"));
    ui->detectOriginBox->setItemText(1, tr("Need Mold Opened Signal"));
    ui->detectOriginBox->setItemText(0, tr("Don't need Mold Opened Signal"));
    ui->label_14->setText(tr("Detect Origin"));
    ui->detectPositionBox->setItemText(0, tr("Vertical"));
    ui->detectPositionBox->setItemText(1, tr("Horizontal"));
    ui->detectPositionBox->setItemText(2, tr("Don't Limit"));
    ui->label_15->setText(tr("Orgin Position"));
    ui->originPositionBox->setItemText(0, tr("Vertical"));
    ui->originPositionBox->setItemText(1, tr("Horizontal"));
    ui->originPositionBox->setItemText(2, tr("No Limit"));
    ui->label_7->setText(tr("IMM Emergency"));
    ui->detectIMMComboBox->setItemText(0, noUse);
    ui->detectIMMComboBox->setItemText(1, use);
    ui->label_16->setText(tr("Down Position"));
    ui->downPositionBox->setItemText(0, tr("Vertical"));
    ui->downPositionBox->setItemText(1, tr("Horizontal"));
    ui->downPositionBox->setItemText(2, tr("No Limit"));
}
