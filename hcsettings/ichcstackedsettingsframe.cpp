#include "ichcstackedsettingsframe.h"
#include "ui_ichcstackedsettingsframe.h"

#include "icmold.h"
#include "icvirtualhost.h"
#include <QTimer>

#include <QDebug>

ICHCStackedSettingsFrame::ICHCStackedSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCStackedSettingsFrame)
{
    ui->setupUi(this);

    InitInterface();
    ui->currentPageLabel->setText(ui->page0ToolButton->text());
    currentPage_ = 0;
    RefreshStackParams_(currentPage_);
    connect(ICMold::CurrentMold(),
            SIGNAL(MoldNumberParamChanged()),
            this,
            SLOT(OnMoldNumberParamChanged()));
}

ICHCStackedSettingsFrame::~ICHCStackedSettingsFrame()
{
    delete ui;
}

void ICHCStackedSettingsFrame::on_page3ToolButton_clicked()
{
    QList<int> status = GetCurrentStatus_();
    SetStackStatus_(status);
    ui->currentPageLabel->setText(ui->page3ToolButton->text());
    currentPage_ = 3;
    RefreshStackParams_(currentPage_);
}

void ICHCStackedSettingsFrame::on_page2ToolButton_clicked()
{

    QList<int> status = GetCurrentStatus_();
    SetStackStatus_(status);
    ui->currentPageLabel->setText(ui->page2ToolButton->text());
    currentPage_ = 2;
    RefreshStackParams_(currentPage_);
}

void ICHCStackedSettingsFrame::on_page1ToolButton_clicked()
{

    QList<int> status = GetCurrentStatus_();
    SetStackStatus_(status);

    ui->currentPageLabel->setText(ui->page1ToolButton->text());
    currentPage_ = 1;
    RefreshStackParams_(currentPage_);
}

void ICHCStackedSettingsFrame::on_page0ToolButton_clicked()
{
    QList<int> status = GetCurrentStatus_();
    SetStackStatus_(status);
    ui->currentPageLabel->setText(ui->page0ToolButton->text());
    currentPage_ = 0;
    RefreshStackParams_(currentPage_);
}

void ICHCStackedSettingsFrame::InitInterface()
{
    QIntValidator * validator = new QIntValidator(0, 65535, this);
    QIntValidator * validator_ = new QIntValidator(0, 65530, this);

    ui->xRPLatticeLineEdit->SetDecimalPlaces(0);
    ui->xRPLatticeLineEdit->setValidator(validator);
    ui->xRPStepLineEdit->SetDecimalPlaces(2);
    ui->xRPStepLineEdit->setValidator(validator_);

    ui->xRPLatticeLineEdit->SetDecimalPlaces(0);
    ui->yRPLatticeLineEdit->setValidator(validator);
    ui->yRPStepLineEdit->SetDecimalPlaces(2);
    ui->yRPStepLineEdit->setValidator(validator_);

    ui->xRPLatticeLineEdit->SetDecimalPlaces(0);
    ui->zRPLatticeLineEdit->setValidator(validator);
    ui->zRPStepLineEdit->SetDecimalPlaces(2);
    ui->zRPStepLineEdit->setValidator(validator_);

//    ui->stackCount->setValidator(validator);

}

void ICHCStackedSettingsFrame::RefreshStackParams_(int group)
{
    QList<int> stackParams = ICMold::CurrentMold()->StackParams(group);
    int seq = stackParams.at(ICMold::Seq);
    int seqH;
    int seqL;
    seqH = seq;
    seqL = seq & 0x000F;
    if(seqL == 0)
    {
        ui->xzyCheckBox->setChecked(true);
    }
    else if(seqL == 1)
    {
        ui->zxyCheckBox->setChecked(true);
    }
    else if(seqL == 2)
    {
        ui->yxzCheckBox->setChecked(true);
    }
    else if(seqL == 3)
    {
        ui->yzxCheckBox->setChecked(true);
    }
    ui->xRPLatticeLineEdit->SetThisIntToThisText(stackParams.at(ICMold::X_Array));
    seqH & 32 ? ui->xRPCheckBox->click() : ui->xPPCheckBox->click();
    ui->xRPStepLineEdit->SetThisIntToThisText(stackParams.at(ICMold::X_Gap));
    ui->yRPLatticeLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Y_Array));
    seqH & 64 ? ui->yRPCheckBox->click() : ui->yPPCheckBox->click();
    ui->yPPCheckBox->setChecked(!ui->yRPCheckBox->isChecked());
    ui->yRPStepLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Y_Gap));
    ui->zRPLatticeLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Z_Array));
    seqH & 128 ? ui->zRPCheckBox->click() : ui->zPPCheckBox->click();
    ui->zPPCheckBox->setChecked(!ui->zRPCheckBox->isChecked());
    ui->zRPStepLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Z_Gap));
    ui->countWayBox->setCurrentIndex(ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(currentPage_)));
//    switch(stackParams.at(ICMold::Seq))
//    {
//    case 64:
//        ui->xzyCheckBox->setChecked(true);
//        break;
//    case 128:
//        break;
//    case 2:
//        ui->zxyCheckBox->setChecked(true);
//        break;
//    case 3:
//        ui->yzxCheckBox->setChecked(true);
    //        ui->yxzCheckBox->setChecked(true);
//    }
}

QList<int> ICHCStackedSettingsFrame::GetCurrentStatus_() const
{
    int seqL = 0;
    int seqH = 0;
    QList<int> status;
    QAbstractButton *checkedButton = ui->buttonGroup->checkedButton();
    if(checkedButton == ui->xzyCheckBox)
    {
        seqL = 0;
    }
    else if(checkedButton == ui->zxyCheckBox)
    {
        seqL = 1;
    }
    else if(checkedButton == ui->yxzCheckBox)
    {
        seqL = 2;
    }
    else
    {
        seqL = 3;
    }
    if(ui->xRPCheckBox->isChecked())
    {
        seqH |= 32;
    }
    if(ui->yRPCheckBox->isChecked())
    {
        seqH |= 64;
    }
    if(ui->zRPCheckBox->isChecked())
    {
        seqH |= 128;
    }

    status.append(seqH + seqL);
    status.append(ui->xRPLatticeLineEdit->TransThisTextToThisInt());
    status.append(ui->yRPLatticeLineEdit->TransThisTextToThisInt());
    status.append(ui->zRPLatticeLineEdit->TransThisTextToThisInt());
    status.append(ui->xRPStepLineEdit->TransThisTextToThisInt());
    status.append(ui->yRPStepLineEdit->TransThisTextToThisInt());
    status.append(ui->zRPStepLineEdit->TransThisTextToThisInt());
    status.append(ui->countWayBox->currentIndex());
//    status.append(ui->xRPCheckBox->isChecked() ? 1 : 0);
//    status.append(ui->yRPCheckBox->isChecked() ? 1 : 0);
//    status.append(ui->zRPCheckBox->isChecked() ? 1 : 0);
    return status;
}

void ICHCStackedSettingsFrame::SetStackStatus_(const QList<int> &status)
{
    ICMold *currentMold = ICMold::CurrentMold();
    const int count = status.size() - 1;
    for(int i = 0; i != count; ++i)
    {
        currentMold->SetStackParam(currentPage_, static_cast<ICMold::ICStatckParam>(i), status.at(i));
    }
    currentMold->SetMoldParam(static_cast<ICMold::ICMoldParam>(currentPage_), status.at(count));
}

void ICHCStackedSettingsFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    qDebug("stack hide");
    QList<int> status = GetCurrentStatus_();
    SetStackStatus_(status);
    ICMold::CurrentMold()->SaveMoldParamsFile();
//    qApp->processEvents(QEventLoop::WaitForMoreEvents);
//    QTimer::singleShot(5, this, SLOT(TimeOutToReconfig()));
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
}

void ICHCStackedSettingsFrame::changeEvent(QEvent *e)
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

void ICHCStackedSettingsFrame::OnMoldNumberParamChanged()
{
    RefreshStackParams_(currentPage_);
}
