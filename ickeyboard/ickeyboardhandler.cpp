#include <QMessageBox>
#include <QKeyEvent>
#include <QApplication>

#include "ickeyboardhandler.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"
#include "iccommandprocessor.h"
#include "mainframe.h"
#include "icvirtualkey.h"
#include "icactioncommand.h"
#include "icparameterssave.h"
//#include "icprogramheadframe.h"
//#include "icmold.h"
//#include "icmacrosubroutine.h"
#include "operatingratiosetdialog.h"

QScopedPointer<ICKeyboardHandler> ICKeyboardHandler::instance_;
ICKeyboardHandler::ICKeyboardHandler(QObject *parent) :
    QObject(parent),
    pulleyTurn_(0)
{
    virtualKeyMap_.insert(ICKeyboard::KS_ManualStatu, IC::CMD_TurnManual);
    virtualKeyMap_.insert(ICKeyboard::KS_StopStatu, IC::CMD_TurnStop);
    virtualKeyMap_.insert(ICKeyboard::KS_AutoStatu, IC::CMD_TurnAuto);
    virtualKeyMap_.insert(ICKeyboard::FB_Down, IC::VKEY_DOWN);
    virtualKeyMap_.insert(ICKeyboard::FB_Up, IC::VKEY_UP);
    virtualKeyMap_.insert(ICKeyboard::FB_Origin, IC::VKEY_ORIGIN);
    virtualKeyMap_.insert(ICKeyboard::VFB_Pose_Horizontal, IC::VKEY_HORI);
    virtualKeyMap_.insert(ICKeyboard::VFB_Pose_Vertical, IC::VKEY_VERT);
//    virtualKeyMap_.insert(ICKeyboard::VFB_Pose_Horizontal, IC::VKEY_CADD);
//    virtualKeyMap_.insert(ICKeyboard::VFB_Pose_Vertical, IC::VKEY_CSUB);
    virtualKeyMap_.insert(ICKeyboard::FB_Reset, IC::VKEY_RETURN);
    virtualKeyMap_.insert(ICKeyboard::VFB_Run, IC::VKEY_START);
    virtualKeyMap_.insert(ICKeyboard::FB_Stop, IC::VKEY_STOP);
    virtualKeyMap_.insert(ICKeyboard::VFB_Y1Sub, IC::VKEY_Y1SUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_Y1Add, IC::VKEY_Y1ADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_Y2Sub, IC::VKEY_Y2SUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_Y2Add, IC::VKEY_Y2ADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_ZAdd, IC::VKEY_ZADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_ZSub, IC::VKEY_ZSUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_X1Sub, IC::VKEY_X1SUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_X1Add, IC::VKEY_X1ADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_X2Sub, IC::VKEY_X2SUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_X2Add, IC::VKEY_X2ADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_SP1, IC::VKEY_SP1);
    virtualKeyMap_.insert(ICKeyboard::VFB_SP2, IC::VKEY_SP2);
    virtualKeyMap_.insert(ICKeyboard::VFB_AAdd, IC::VKEY_AADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_ASub, IC::VKEY_ASUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_BAdd, IC::VKEY_BADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_BSub, IC::VKEY_BSUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_CAdd, IC::VKEY_CADD);
    virtualKeyMap_.insert(ICKeyboard::VFB_CSub, IC::VKEY_CSUB);
    virtualKeyMap_.insert(ICKeyboard::VFB_SingleStep, IC::VKEY_F6);
    virtualKeyMap_.insert(ICKeyboard::FB_NULL, IC::VKEY_NULL);
}

void ICKeyboardHandler::SwitchChanged(int value)
{
    if(value < 0)
    {
        return;
    }
    Q_ASSERT_X(virtualKeyMap_.contains(value),
               "ICKeyboardHandle::Keypressed",
               (QString::number(value) + " is not define").toAscii());
    //    ICCommandProcessor* commandProcessor = ICCommandProcessor::Instance();
    //    if(value == ICKeyboard::KS_ManualStatu
    //       || value == ICKeyboard::KS_StopStatu
    //       || value == ICKeyboard::KS_AutoStatu)
    //    {
    //        commandProcessor->ExecuteHCCommand(virtualKeyMap_.value(value),
    //                                           0,
    //                                           ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
    //                                           ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());
    //    }
//    qDebug()<<"swkey:"<<value;
    if(value == ICKeyboard::KS_AutoStatu)
    {
//        if(icInstructionPage != NULL)
//        {
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
//        ICProgramHeadFrame::Instance()->StartAutoTime();
            icMainFrame->ShowAutoPage();
//            icInstructionPage->ShowProgramMonitor();
//        }
    }
    else if(value == ICKeyboard::KS_ManualStatu)
    {
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
        qDebug("know manual");
        icMainFrame->ShowManualPage();
    }
    else
    {
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
        icMainFrame->ShowStandbyPage();
    }
}

void ICKeyboardHandler::Keypressed(int keyValue)
{
    if(keyValue < 0 || keyValue > 50)
    {
        return;
    }
    bool isBackLight = icMainFrame->IsBackLightOff();
    icMainFrame->SetHasInput(true);
    if(isBackLight)
    {
        return;
    }
    Q_ASSERT_X(virtualKeyMap_.contains(keyValue),
               "ICKeyboardHandle::Keypressed",
               (QString::number(keyValue) + " is not define").toAscii());


    static ICVirtualHost* virtualHost = ICVirtualHost::GlobalVirtualHost();
    uint status = virtualHost->CurrentStatus();
    ICCommandProcessor* commandProcessor = ICCommandProcessor::Instance();
    if(keyValue == ICKeyboard::VFB_Run)
    {
        commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
        if((!icMainFrame->IsOrigined()) && icMainFrame->IsAutoPageShown())
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Need to origin!"));
        }

        return;
    }
    if(keyValue == ICKeyboard::FB_NULL)
    {
        commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
        return;
    }

    if( keyValue == ICKeyboard::FB_Down ||
            keyValue == ICKeyboard::FB_Up)
    {
        int currentTuneSpeedType = ICKeyboard::Instace()->CurrentTuneSpeedType();
        if(currentTuneSpeedType < 0) return;
        if(status == ICVirtualHost::Auto)
        {
            ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
            if(!host->IsSpeedEnable())
            {
                return;
            }
            int currentSpeed =  virtualHost->GlobalSpeed();
            int xSpeed = currentSpeed >> 8 & 0x000000FF;
            int ySpeed = currentSpeed & 0xFF;
#ifdef HC_AXIS_COUNT_5
            int zSpeed = (currentSpeed >> 16) & 0xFF;
#endif
            int currentSpeedStep = OperatingRatioSetDialog::Instance()->CurrentGlobalSpeedStep();
            if(keyValue == ICKeyboard::FB_Down)
            {
                if(currentTuneSpeedType == 0) xSpeed -=currentSpeedStep;
                else if(currentTuneSpeedType == 1)ySpeed -= currentSpeedStep;
#ifdef HC_AXIS_COUNT_5
                else if(currentTuneSpeedType == 2) zSpeed -= currentSpeedStep;
#endif
            }
            else
            {
                if(currentTuneSpeedType == 0) xSpeed +=currentSpeedStep;
                else if(currentTuneSpeedType == 1)ySpeed += currentSpeedStep;
#ifdef HC_AXIS_COUNT_5
                else if(currentTuneSpeedType == 2) zSpeed += currentSpeedStep;
#endif
            }
            if(xSpeed < 10)
            {
                xSpeed = 10;
            }
            if(ySpeed < 10)
            {
                ySpeed = 10;
            }
#ifdef HC_AXIS_COUNT_5
            if(zSpeed < 10) zSpeed = 10;
#endif
            if(xSpeed > 100)
            {
                xSpeed = 100;
            }
            if(ySpeed > 100)
            {
                ySpeed = 100;
            }
#ifdef HC_AXIS_COUNT_5
            if(zSpeed > 100) zSpeed = 100;
#endif

            currentSpeed = (xSpeed << 8) | ySpeed;
#ifdef HC_AXIS_COUNT_5
            currentSpeed |= (zSpeed << 16);
#endif

            host->SetGlobalSpeed(currentSpeed);
            host->SetTuneSpeed(true);
        }
        else
        {
            if(currentTuneSpeedType == 0)
            {
                commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
            }
            else if(currentTuneSpeedType == 1)
            {
                if(keyValue == ICKeyboard::FB_Up) commandProcessor->ExecuteVirtualKeyCommand(IC::VKEY_Y_SPEED_UP);
                else commandProcessor->ExecuteVirtualKeyCommand(IC::VKEY_Y_SPEED_DOWN);
            }
            else if(currentTuneSpeedType == 2)
            {
                if(keyValue == ICKeyboard::FB_Up) commandProcessor->ExecuteVirtualKeyCommand(IC::VKEY_Z_SPEED_UP);
                else commandProcessor->ExecuteVirtualKeyCommand(IC::VKEY_Z_SPEED_DOWN);
            }
        }
        return;
    }

    if(keyValue == ICKeyboard::FB_Stop)
    {
        if(icMainFrame->IsOrigin())
        {
            //            SwitchChanged(ICKeyboard::KS_StopStatu);
            //            icMainFrame->HideOrigin();
            commandProcessor->ExecuteHCCommand(IC::CMD_TurnStop, 0);
        }
        else if(ICVirtualHost::GlobalVirtualHost()->IsSingleRun())
        {
            ICVirtualHost::GlobalVirtualHost()->SetSingleRun(false);
            commandProcessor->ExecuteVirtualKeyCommand(IC::VKEY_STOP);
        }
        else
        {
            commandProcessor->ExecuteVirtualKeyCommand(IC::VKEY_STOP);
        }
        return;
    }

    //    if(status == ICVirtualHost::Auto) //execute Auto status
    //    {
    //        if(keyValue == ICKeyboard::FB_Stop)
    //        {
    //            commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
    //        }
    //        return;
    //    }
    switch(keyValue)
    {
    case ICKeyboard::FB_Origin:
    {
        if(status != ICVirtualHost::Stop)
        {
            return;
        }
        //            commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
        if(commandProcessor->ExecuteHCCommand(IC::CMD_TurnZero, virtualKeyMap_.value(keyValue)))
        {
            //                icMainFrame->ShowOrigin();
        }
    }
        break;
    case ICKeyboard::FB_Reset:
    {
        if(status != ICVirtualHost::Stop)
        {
            return;
        }
        //            commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
        commandProcessor->ExecuteHCCommand(IC::CMD_TurnRet, virtualKeyMap_.value(keyValue));

    }
        break;
    case ICKeyboard::VFB_X2Add:
    case ICKeyboard::VFB_X2Sub:
    case ICKeyboard::VFB_Y2Add:
    case ICKeyboard::VFB_Y2Sub:
    {
//        if(!ICParametersSave::Instance()->IsSingleArm())
        {
            if(status == ICVirtualHost::Stop)
            {
                return;
            }
            commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
        }
    }
    break;
    case ICKeyboard::VFB_Pose_Horizontal:
    case ICKeyboard::VFB_Pose_Vertical:
    {
        if(status == ICVirtualHost::Stop)
        {
            return;
        }
        commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
    }
    break;
    default:
    {
        if(status == ICVirtualHost::Stop)
        {
            return;
        }
        else if(status == ICVirtualHost::Manual)
        {
//            if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
//            {
//                QMessageBox::warning(NULL, tr("Warning"), tr("Need to origin!"));
//            }
        }
        commandProcessor->ExecuteVirtualKeyCommand(virtualKeyMap_.value(keyValue));
    }
    }
}

void ICKeyboardHandler::PulleyChanged(int value)
{
//    qDebug("in pullye changed");
    if(value == 0)
    {
        return;
    }
    int status = ICVirtualHost::GlobalVirtualHost()->CurrentStatus();
    if(status != ICVirtualHost::Manual)
    {
        return;
    }
    qDebug("status is right");
    pulleyTurn_ = (pulleyTurn_ + 1) % 2;
    int currentPulleySpeed = OperatingRatioSetDialog::Instance()->CurrentPulleySpeed();
    int cmd;
    if(pulleyTurn_ == 0)
    {
        for(int i = 0; i != currentPulleySpeed; ++i)
        {
//            qDebug("pulseA");
            cmd = IC::CMD_PulseA + i % 2;
            ICCommandProcessor::Instance()->ExecuteHCCommand(cmd, value);
        }
    }
    else
    {
        for(int i = 0; i != currentPulleySpeed; ++i)
        {
//            qDebug("pulseB");
            cmd = IC::CMD_PulseB - i % 2;
            ICCommandProcessor::Instance()->ExecuteHCCommand(cmd, value);
        }

    }
}
