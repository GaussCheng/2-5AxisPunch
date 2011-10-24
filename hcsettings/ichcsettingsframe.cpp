#include <QButtonGroup>
#include "ichcsettingsframe.h"
#include "ui_ichcsettingsframe.h"

#include "axissettingsframe.h"
#include "icstructdefineframe.h"
#include "icvirtualhost.h"
#include "icparameterssave.h"
#include "icprogramheadframe.h"

ICHCSettingsFrame::ICHCSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCSettingsFrame),
    centralStackedLayout_(new QStackedLayout)
{
    ui->setupUi(this);
    buttonGroup_ = new QButtonGroup();

    InitHCSettingPage();
    InitSignal();
//    ui->basicSettingsToolButton->setText(tr("Setting"));
    ui->axisXToolButton->setText(tr("X Axis"));
    ui->axisYToolButton->setText(tr("Y Axis"));
    ui->axisZToolButton->setText(tr("Z Axis"));
//    buttonGroup_->addButton(ui->basicSettingsToolButton);
    buttonGroup_->addButton(ui->axisXToolButton);
    buttonGroup_->addButton(ui->axisYToolButton);
    buttonGroup_->addButton(ui->axisZToolButton);
    buttonGroup_->setExclusive(true);
    QAbstractButton* button;
    foreach(button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }
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
        ui->axisXToolButton->setText(tr("X Axis"));
        ui->axisYToolButton->setText(tr("Y Axis"));
        ui->axisZToolButton->setText(tr("Z Axis"));
    }
        break;
    default:
        break;
    }
}

void ICHCSettingsFrame::showEvent(QShowEvent *e)
{
    ui->axisXToolButton->click();
    QFrame::showEvent(e);
}

void ICHCSettingsFrame::hideEvent(QHideEvent *e)
{
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
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
    structDefPage_ = new ICStructDefineFrame();
    centralStackedLayout_->addWidget(structDefPage_);
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

void ICHCSettingsFrame::on_axisXToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisXToolButton->text(), AxisX);
}

void ICHCSettingsFrame::on_axisYToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisYToolButton->text(), AxisY);
}

void ICHCSettingsFrame::on_axisZToolButton_clicked()
{
    ShowAxisSettingPage(ui->axisZToolButton->text(), AxisZ);
}

void ICHCSettingsFrame::on_structDefButton_clicked()
{
    centralStackedLayout_->setCurrentWidget(structDefPage_);
}

void ICHCSettingsFrame::ShowAxisSettingPage(QString currentAxisName, int axis)
{
    centralStackedLayout_->setCurrentWidget(axisSettingsPage_);
    emit CurrentAxisChanged(currentAxisName, axis);
}
