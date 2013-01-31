#include <QButtonGroup>
#include "ichcsettingsframe.h"
#include "ui_ichcsettingsframe.h"

#include "axissettingsframe.h"
#include "icstructdefineframe.h"
#include "icvirtualhost.h"
#include "icparameterssave.h"
#include "icprogramheadframe.h"
#include "icmachinefigure.h"

ICHCSettingsFrame::ICHCSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCSettingsFrame),
    centralStackedLayout_(new QStackedLayout)
{
    ui->setupUi(this);
//    ui->structDefButton->hide();
    buttonGroup_ = new QButtonGroup();

    InitHCSettingPage();
    InitSignal();
//    ui->basicSettingsToolButton->setText(tr("Setting"));
    ui->axisX1ToolButton->setText(tr("X1 Axis"));
    ui->axisY1ToolButton->setText(tr("Y1 Axis"));
    ui->axisZToolButton->setText(tr("Z Axis"));
    ui->axisX2ToolButton->setText(tr("X2 Axis"));
    ui->axisY2ToolButton->setText(tr("Y2 Axis"));
    ui->axisAToolButton->setText(tr("A Axis"));
    ui->axisBToolButton->setText(tr("B Axis"));
    ui->axisCToolButton->setText(tr("C Axis"));
    ui->machineFigureButton->setText(tr("Figure"));
//    buttonGroup_->addButton(ui->basicSettingsToolButton);
    buttonGroup_->addButton(ui->axisX1ToolButton);
    buttonGroup_->addButton(ui->axisY1ToolButton);
    buttonGroup_->addButton(ui->axisZToolButton);
    buttonGroup_->addButton(ui->axisX2ToolButton);
    buttonGroup_->addButton(ui->axisY2ToolButton);
    buttonGroup_->addButton(ui->axisAToolButton);
    buttonGroup_->addButton(ui->axisBToolButton);
    buttonGroup_->addButton(ui->axisCToolButton);
    buttonGroup_->addButton(ui->machineFigureButton);
    buttonGroup_->setExclusive(true);
    QAbstractButton* button;
    foreach(button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }
//    connect(ICProgramHeadFrame::Instance(),
//            SIGNAL(LevelChanged(int)),
//            this,
//            SLOT(LevelChanged(int)));
//    if(ICProgramHeadFrame::Instance()->CurrentLevel() == ICParametersSave::AdvanceAdmin)
//    {
//        LevelChanged(ICParametersSave::AdvanceAdmin);
//    }
//    ui->axisXToolButton->click();
//    ui->basicSettingsToolButton->click();
}

ICHCSettingsFrame::~ICHCSettingsFrame()
{
    delete ui;
}

void ICHCSettingsFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
//        ui->basicSettingsToolButton->setText(tr("Setting"));
        ui->axisX1ToolButton->setText(tr("X1 Axis"));
        ui->axisY1ToolButton->setText(tr("Y1 Axis"));
        ui->axisZToolButton->setText(tr("Z Axis"));
        ui->axisX2ToolButton->setText(tr("X2 Axis"));
        ui->axisY2ToolButton->setText(tr("Y2 Axis"));
        ui->axisAToolButton->setText(tr("A Axis"));
        ui->axisBToolButton->setText(tr("B Axis"));
        ui->axisCToolButton->setText(tr("C Axis"));
        ui->machineFigureButton->setText(tr("Figure"));
    }
        break;
    default:
        break;
    }
}

void ICHCSettingsFrame::showEvent(QShowEvent *e)
{
    UpdateAxisDefine_();
    if(!ui->axisX1ToolButton->isHidden())
    {
        ui->axisX1ToolButton->click();
    }
    else if(!ui->axisY1ToolButton->isHidden())
    {
        ui->axisY1ToolButton->click();
    }
    else if(!ui->axisX2ToolButton->isHidden())
    {
        ui->axisX2ToolButton->click();
    }
    else if(!ui->axisY2ToolButton->isHidden())
    {
        ui->axisY2ToolButton->click();
    }
    else if(!ui->axisAToolButton->isHidden())
    {
        ui->axisAToolButton->click();
    }
    else if(!ui->axisBToolButton->isHidden())
    {
        ui->axisBToolButton->click();
    }
    else if(!ui->axisCToolButton->isHidden())
    {
        ui->axisCToolButton->click();
    }
    else if(!ui->machineFigureButton->isHidden())
    {
        ui->machineFigureButton->click();
    }
    QFrame::showEvent(e);
}

void ICHCSettingsFrame::hideEvent(QHideEvent *e)
{
    if(ICVirtualHost::GlobalVirtualHost()->IsParamChanged())
    {
        /***********BUG#200**********************/
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    QFrame::hideEvent(e);
}

//void ICHCSettingsFrame::SettingButtonClicked()
//{
//    QAbstractButton * toolButton = qobject_cast<QAbstractButton *>(sender());
//    if(toolButton == ui->basicSettingsToolButton)
//    {
//        centralStackedLayout_->setCurrentWidget(basicSettingsPage_);
//    }
//    else
//    {
//        centralStackedLayout_->setCurrentWidget(axisSettingsPage_);
//        int axis = -1;
//        if(sender() == ui->axisXToolButton)
//        {
//        }
//        else if(sender() == ui->)
//        emit CurrentAxisChanged(toolButton->text(), );
//    }
//}

void ICHCSettingsFrame::InitHCSettingPage()
{
    ui->settingStackedFrame->setLayout(centralStackedLayout_);

    axisSettingsPage_ = new AxisSettingsFrame;
    centralStackedLayout_->addWidget(axisSettingsPage_);
    machineFigure_ = new ICMachineFigure;
    centralStackedLayout_->addWidget(machineFigure_);
//    structDefPage_ = new ICStructDefineFrame();
//    centralStackedLayout_->addWidget(structDefPage_);
//    basicSettingsPage_ =  new BasicSettingsFrame;
//    centralStackedLayout_->addWidget(basicSettingsPage_);

//    centralStackedLayout_->setCurrentWidget(basicSettingsPage_);
}

void ICHCSettingsFrame::InitSignal()
{
    connect(this,
            SIGNAL(CurrentAxisChanged(QString, int)),
            axisSettingsPage_,
            SLOT(SetCurrentAxis(QString, int)));

//    connect(ui->basicSettingsToolButton,
//            SIGNAL(clicked()),
//            this,
//            SLOT(SettingButtonClicked()));

    /*connect(ui->axisXToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->axisYToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->axisZToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));*/
}

void ICHCSettingsFrame::on_axisX1ToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisX1ToolButton->text(), ICVirtualHost::ICAxis_AxisX1);
}

void ICHCSettingsFrame::on_axisY1ToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisY1ToolButton->text(), ICVirtualHost::ICAxis_AxisY1);
}

void ICHCSettingsFrame::on_axisZToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisZToolButton->text(), ICVirtualHost::ICAxis_AxisZ);
}

void ICHCSettingsFrame::on_axisX2ToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisX2ToolButton->text(), ICVirtualHost::ICAxis_AxisX2);
}

void ICHCSettingsFrame::on_axisY2ToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisY2ToolButton->text(), ICVirtualHost::ICAxis_AxisY2);
}

void ICHCSettingsFrame::on_axisAToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisAToolButton->text(), ICVirtualHost::ICAxis_AxisA);
}

void ICHCSettingsFrame::on_axisBToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisBToolButton->text(), ICVirtualHost::ICAxis_AxisB);
}

void ICHCSettingsFrame::on_axisCToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisCToolButton->text(), ICVirtualHost::ICAxis_AxisC);
}

void ICHCSettingsFrame::on_machineFigureButton_clicked()
{
    centralStackedLayout_->setCurrentWidget(machineFigure_);
}

void ICHCSettingsFrame::ShowAxisSettingPage(QString currentAxisName, int axis)
{
    centralStackedLayout_->setCurrentWidget(axisSettingsPage_);
    emit CurrentAxisChanged(currentAxisName, axis);
}

void ICHCSettingsFrame::UpdateAxisDefine_()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int currentAxis = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
    if(axisDefine_ != currentAxis)
    {
        axisDefine_ = currentAxis;
        ui->axisAToolButton->hide();
        ui->axisBToolButton->hide();
        ui->axisCToolButton->hide();
        ui->axisX1ToolButton->hide();
        ui->axisX2ToolButton->hide();
        ui->axisY1ToolButton->hide();
        ui->axisY2ToolButton->hide();
        ui->axisZToolButton->hide();

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisX1ToolButton->hide();
        }
        else
        {
            ui->axisX1ToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisY1ToolButton->hide();
        }
        else
        {
            ui->axisY1ToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisZToolButton->hide();
        }
        else
        {
            ui->axisZToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisX2ToolButton->hide();
        }
        else
        {
            ui->axisX2ToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisY2ToolButton->hide();
        }
        else
        {
            ui->axisY2ToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisAToolButton->hide();
        }
        else
        {
            ui->axisAToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisBToolButton->hide();
        }
        else
        {
            ui->axisBToolButton->show();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->axisCToolButton->hide();
        }
        else
        {
            ui->axisCToolButton->show();
        }
    }
}

//void ICHCSettingsFrame::LevelChanged(int level)
//{
//    if(level == ICParametersSave::AdvanceAdmin)
//    {
//        ui->structDefButton->show();
//    }
//    else
//    {
//        ui->structDefButton->hide();
//    }
//}
