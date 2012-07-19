#include "ichostcomparepage.h"
#include "ui_ichostcomparepage.h"

#include <QMessageBox>
#include "icvirtualhost.h"
#include "icparameterssave.h"

ICHostComparePage::ICHostComparePage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICHostComparePage)
{
    ui->setupUi(this);
}

ICHostComparePage::~ICHostComparePage()
{
    delete ui;
}

void ICHostComparePage::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICHostComparePage::on_hostBtn_clicked()
{
    int ret = QMessageBox::warning(this,
                                   tr("Warning"),
                                   tr("Do you really want to check out all the host machine configures?"),
                                   QMessageBox::No | QMessageBox::Yes);
    if(ret == QMessageBox::No)
    {
        rejected();
        return;
    }
    ICCommandProcessor* process = ICCommandProcessor::Instance();
    ICGetAxisConfigsCommand command;
    command.SetSlave(process->SlaveID());
    int addr = ICVirtualHost::SYS_X_Length;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ICParametersSave *pS = ICParametersSave::Instance();
    QString axis[] = {"X", "Y", "Z", "P", "Q", "A", "B", "C"};
    for(int i = 0; i != 9; ++i)
    {
        command.SetAxis(i);
        ICCommunicationCommandBase::ResultVector ret = process->ExecuteCommand(command).value<ICCommunicationCommandBase::ResultVector>();
        if(!ret.isEmpty())
        {
            host->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++), ret.at(0));
            host->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++), ret.at(1));
            host->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++), ret.at(2));
            host->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++), ret.at(3));
            host->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++), ret.at(4));
            host->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++), ret.at(5));
            host->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++), ret.at(6));
            if( i < 8)
            {
                pS->SetDistanceRotation(axis[i], (ret.at(0) * 1000) / (uint)((ret.at(5) << 16) | ret.at(4)));
                ICVirtualHost::GlobalVirtualHost()->SaveAxisParam(i);
            }
        }
        else
        {
            QMessageBox::critical(this,
                                  tr("Critical"),
                                  tr("Operator fail! Please reboot system"));
            break;
        }
    }
    ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
    accepted();
}

void ICHostComparePage::on_hmiBtn_clicked()
{
    int ret = QMessageBox::warning(this,
                                   tr("Warning"),
                                   tr("Do you really want to rewrite all the machine configures to host?"),
                                   QMessageBox::No | QMessageBox::Yes);
    if(ret == QMessageBox::No)
    {
        rejected();
        return;
    }

    ICSetAxisConfigsCommand command;
    ICCommandProcessor* processor = ICCommandProcessor::Instance();
    command.SetSlave(processor->SlaveID());
    QList<uint> buffer;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int addr = ICVirtualHost::SYS_X_Length;
    for(int i = 0; i != 9; ++i)
    {
        buffer.clear();
        command.SetAxis(i);
        buffer.append(host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++)).toUInt());
        buffer.append(host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++)).toUInt());
        buffer.append(host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++)).toUInt());
        buffer.append(host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++)).toUInt());
        buffer.append(host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++)).toUInt());
        buffer.append(host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++)).toUInt());
        buffer.append(host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(addr++)).toUInt());
        command.SetDataBuffer(buffer.toVector());
        if(!processor->ExecuteCommand(command).toBool())
        {
            QMessageBox::critical(this,
                                  tr("Critical"),
                                  tr("Operator fail! Please reboot system"));
            break;
            //            host->SetSystemParameter(machineLangth, status.at(0));
            //            host->SetSystemParameter(maxLangth, status.at(1));
            //            host->SetSystemParameter(iSafe, status.at(2));
            //            host->SetSystemParameter(oSafe, status.at(3));
            //            host->SetSystemParameter(tL, status.at(4));
            //            host->SetSystemParameter(tH, status.at(5));
            //            host->SetSystemParameter(sum, status.at(6));
            //            ICParametersSave::Instance()->SetDistanceRotation(axisName, ui->distanceRotationEdit->text().toDouble());
        }
    }
    accepted();
}

void ICHostComparePage::on_cancelBtn_clicked()
{
    rejected();
}
