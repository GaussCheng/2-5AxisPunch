#include "ichcproductsettingframe.h"
#include "ui_ichcproductsettingframe.h"

#include "iclineeditwrapper.h"
#include "icmold.h"
#include "icvirtualhost.h"
#include "icvirtualkey.h"

ICHCProductSettingFrame::ICHCProductSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCProductSettingFrame)
{
    ui->setupUi(this);

    ui->productLineEdit->setValidator(new QIntValidator(0, 65535, ui->productLineEdit));
    ui->alarmTimesEdit->setValidator(new QIntValidator(0, 65535, ui->alarmTimesEdit));
    ui->waitTimeEdit->SetDecimalPlaces(1);
    ui->waitTimeEdit->setValidator(new QIntValidator(0, 60000, ui->waitTimeEdit));
    ICLineEditWrapper *wrapper = new ICLineEditWrapper(ui->productLineEdit,
                                                       ICMold::Product,
                                                       ICLineEditWrapper::Mold,
                                                       ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->waitTimeEdit,
                                    ICVirtualHost::SM_WaitMoldOpenLimit,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::OneFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->alarmTimesEdit,
                                    ICVirtualHost::SM_ACCTIME,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    int currentPos = ICMold::CurrentMold()->MoldParam(ICMold::PosMainDown);
    if(currentPos > 1)
    {
        currentPos = 1;
    }
    ui->fixtureSelectBox->setCurrentIndex(ICVirtualHost::GlobalVirtualHost()->FixtureDefine());
    connect(ICMold::CurrentMold(),
            SIGNAL(MoldNumberParamChanged()),
            this,
            SLOT(OnMoldNumberParamChanged()));
}

ICHCProductSettingFrame::~ICHCProductSettingFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void ICHCProductSettingFrame::hideEvent(QHideEvent *e)
{
    qDebug("Product hide");
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->IsParamChanged())
    {
        ICSetAxisConfigsCommand command;
        ICCommandProcessor* process = ICCommandProcessor::Instance();
        int sum = 0;
        QVector<uint> dataBuffer(7, 0);
        dataBuffer[0] = host->SystemParameter(ICVirtualHost::SYS_Config_Signal).toUInt();
        dataBuffer[1] = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toUInt();
        dataBuffer[2] = host->SystemParameter(ICVirtualHost::SYS_Config_Out).toUInt();
        dataBuffer[3] = host->SystemParameter(ICVirtualHost::SYS_Config_Fixture).toUInt();
    //    dataBuffer[3] = ICVirtualHost::GlobalVirtualHost()->FixtureDefine();
        for(int i = 0; i != 6; ++i)
        {
            sum += dataBuffer.at(i);
        }
        sum = (-sum & 0XFFFF);
        dataBuffer[6] = sum;
        qDebug()<<sum;
        command.SetSlave(process->SlaveID());
        command.SetDataBuffer(dataBuffer);
        command.SetAxis(8);
    #ifndef Q_WS_X11
        if(process->ExecuteCommand(command).toBool())
    #endif
        {
            ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
            host->SetSystemParameter(ICVirtualHost::SYS_Config_Signal, dataBuffer.at(0));
            host->SetAxisDefine(dataBuffer.at(1));
            host->SetPeripheryOutput(dataBuffer.at(2));
            host->SetSystemParameter(ICVirtualHost::SYS_Config_Fixture, dataBuffer.at(3));
            host->SetSystemParameter(ICVirtualHost::SYS_Config_Resv1, dataBuffer.at(4));
            host->SetSystemParameter(ICVirtualHost::SYS_Config_Resv2, dataBuffer.at(5));
            host->SetSystemParameter(ICVirtualHost::SYS_Config_Xorsum, dataBuffer.at(6));
        }
        ICMold::CurrentMold()->SaveMoldParamsFile();
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    QFrame::hideEvent(e);
}

void ICHCProductSettingFrame::changeEvent(QEvent *e)
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

void ICHCProductSettingFrame::OnMoldNumberParamChanged()
{
    for(int i = 0; i != wrappers_.size(); ++i)
    {
        wrappers_[i]->UpdateParam();
    }
}

void ICHCProductSettingFrame::on_productClearButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_PRODUCT_CLEAR);
    ICVirtualHost::GlobalVirtualHost()->SetFinishProductCount(0);
}

void ICHCProductSettingFrame::on_fixtureSelectBox_currentIndexChanged(int index)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    host->SetSystemParameter(ICVirtualHost::SYS_Config_Fixture, host->FixtureDefineSwitch(index));
}
