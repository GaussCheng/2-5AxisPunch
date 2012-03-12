#include "icinstructparam.h"

#include <QObject>

#include "icparameterconversion.h"
#include "icparameterssave.h"
#include "moldinformation.h"
#include "icmacrosubroutine.h"

#include <QDebug>

ICInstructParam * ICInstructParam::instance_ = NULL;
QMap<int, QString> ICInstructParam::actionGroupMap_;
QMap<int, QString> ICInstructParam::clipGroupMap_;
QList<int> ICInstructParam::xyzStatusList_;
QList<int> ICInstructParam::clipStatusList_;

ICInstructParam::ICInstructParam()
{
    InstallMoldInfo();

//    connect(ICParametersSave::Instance(),
//            SIGNAL(CurrentLanguageChanged()),
//            this,
//            SLOT(UpdateTranslate()));

    InitClassesInfo();
//    UpdateHostParam();
}

QString ICInstructParam::ConvertCommandStr(const ICMoldItem & moldItem)
{
    QString commandStr;
//    commandStr += QString::number(moldItem.Seq()) + " ";
    if(moldItem.SubNum() == 255)
    {
        commandStr += QString::number(moldItem.Num()) + "    " + "*" + "    ";
    }
    else
    {
        commandStr += QString::number(moldItem.Num()) + "    " + QString::number(moldItem.SubNum()) + "    ";
    }
//    if(moldItem.GMVal() == 31)
//    {
//        return commandStr += actionGroupMap_.value(31);
//    }

    bool clipGroup = moldItem.IsClip();
    uint action;

    if(!clipGroup)
    {
        action = moldItem.Action();

        if(action == ACT_Cut)
        {
            if(moldItem.SVal() < 4)
            {
                commandStr += tr("Fixture");
                commandStr += QString::number(moldItem.SVal() + 1);
            }
            else
            {
                commandStr += tr("Sucker");
                commandStr += QString::number(moldItem.SVal() - 3);
            }
//            commandStr += tr("Fixture");
//            commandStr += QString::number(moldItem.SVal() + 1);
            commandStr += " ";
            if(moldItem.IFVal() == 1)
            {
                commandStr += tr("Begin");
            }
            else
            {
                commandStr += tr("End");
            }
            commandStr += actionGroupMap_.value(action);
            return commandStr;
        }
        commandStr += actionGroupMap_.value(action) + ": ";

        if(xyzStatusList_.contains(action))
        {
            commandStr += ICParameterConversion::TransThisIntToThisText(moldItem.Pos(), 1) + " ";
//            commandStr += "X" + ICParameterConversion::TransThisIntToThisText(moldItem.X(), 1) + " ";
//            commandStr += "Y" + ICParameterConversion::TransThisIntToThisText(moldItem.Y(), 1) + " ";
//            commandStr += "Z" + ICParameterConversion::TransThisIntToThisText(moldItem.Z(), 1) + " ";
//            if(action != G01)
//            {
//                commandStr == QObject::tr("Radius:") + ICParameterConversion::TransThisIntToThisText(moldItem.RVal(), 1);
//            }
            commandStr += tr("Speed:") + QString::number(moldItem.SVal()) + " ";
            if(moldItem.IFVal() == 1)
            {
                commandStr += tr("Early End:");
                commandStr += QString().sprintf("%.2f", moldItem.IFPos() / (qreal)10) + " ";
            }
//            commandStr += tr("Delay")
        }
        else if(action == ACTCHECKINPUT)
        {
//            drs 
            if(moldItem.IFVal() == 0)
            {
                commandStr += tr("Defective Products");
            }
            else if(moldItem.IFVal() == 1)
            {
                commandStr += tr("X043");
            }
            else
            {
                commandStr += tr("X044");
            }
            commandStr += " ";
//            commandStr += " " + tr("ON:Macro") + QString::number(moldItem.RVal()) + " ";
            commandStr += tr("Macro") + QString::number(moldItem.SVal()) + " ";
            commandStr += tr("Return Line") + QString::number(moldItem.DVal());
            return commandStr;
        }
        else if(action == ACT_WaitMoldOpened)
        {
            if(moldItem.SVal() == 1)
            {
                commandStr += tr("Mold Opened");
            }
            else if(moldItem.SVal() == 2)
            {
                commandStr += tr("Security Door Closed");
            }
            else if(moldItem.SVal() == 3)
            {
                commandStr += tr("X043");
            }
            else if(moldItem.SVal() == 4)
            {
                commandStr += tr("X044");
            }
            return commandStr;
        }
    }
    else
    {
        action = moldItem.Clip();
        commandStr += clipGroupMap_.value(action) + ": ";
        if(clipStatusList_.contains(action))
        {
            commandStr += QObject::tr("Times:") + QString::number(moldItem.SVal()) + " ";
        }
        else if(IsStackedAction(action))
        {
            commandStr += QObject::tr("Selected:") + QString::number(moldItem.SVal()) + " ";
        }
        else if(action == ICMold::ACT_AUX1 ||
                action == ICMold::ACT_AUX2 ||
                action == ICMold::ACT_AUX3 ||
                action == ICMold::ACT_AUX4 ||
                action == ICMold::ACT_AUX5 ||
                action == ICMold::ACT_AUX6)
        {
            if(moldItem.IFVal() == 0)
            {
                commandStr += QObject::tr("Off");
            }
            else
            {
                commandStr += QObject::tr("On");
            }
        }

    }

    commandStr += QObject::tr("Delay time:") + ICParameterConversion::TransThisIntToThisText(moldItem.DVal(), 2) + "      ";

    return commandStr;
}

void ICInstructParam::AppendCommandStrList(const QString & commandStr)
{
    commandStrList_.append(commandStr);;
}

void ICInstructParam::InstallMoldInfo()
{
    actionGroupMap_[GC] = QObject::tr("C");
    actionGroupMap_[GX] = QObject::tr("X1");
    actionGroupMap_[GY] = QObject::tr("Y1");
    actionGroupMap_[GZ] = QObject::tr("Z");
    actionGroupMap_[GP] = QObject::tr("X2");
    actionGroupMap_[GQ] = QObject::tr("Y2");
    actionGroupMap_[GA] = QObject::tr("A");
    actionGroupMap_[GB] = QObject::tr("B");
    actionGroupMap_[ACTMAINUP] = QObject::tr("Main arm up");
    actionGroupMap_[ACTMAINDOWN] = QObject::tr("Main arm down");
    actionGroupMap_[ACTMAINFORWARD] = QObject::tr("Main arm forward");
    actionGroupMap_[ACTMAINBACKWARD] = QObject::tr("Main arm backward");
    actionGroupMap_[ACTPOSEHORI] = QObject::tr("Horizontal posture");
    actionGroupMap_[ACTPOSEVERT] = QObject::tr("Vertical posture");
    actionGroupMap_[ACT_PoseHori2] = QObject::tr("Horizontal posture 2");
    actionGroupMap_[ACT_PoseVert2] = QObject::tr("Vertical posture 2");
    actionGroupMap_[ACTVICEUP] = QObject::tr("Vice arm up");
    actionGroupMap_[ACTVICEDOWN] = QObject::tr("Vice arm down");
    actionGroupMap_[ACTVICEFORWARD] = QObject::tr("Vice arm forward");
    actionGroupMap_[ACTVICEBACKWARD] = QObject::tr("Vice arm backward");
    actionGroupMap_[ACTGOOUT] = QObject::tr("Traverse out");
    actionGroupMap_[ACTCOMEIN] = QObject::tr("Traverse in");

//    actionGroupMap_.insert(ACTMAINMIDDOWN, QObject::tr("Main mid down"));
    actionGroupMap_.insert(ACTCHECKINPUT, QObject::tr("Check"));
    actionGroupMap_.insert(ACTEND, QObject::tr("Mold end"));
    actionGroupMap_.insert(ACTParallel, QObject::tr("Parallel"));
    actionGroupMap_.insert(ACT_WaitMoldOpened, QObject::tr("Wait"));
    actionGroupMap_.insert(ACT_Cut, QObject::tr("Cut"));

    clipGroupMap_[ACTCLIP1ON] = QObject::tr("Clip1 ON");
    clipGroupMap_[ACTCLIP2ON] = QObject::tr("Clip2 ON");
    clipGroupMap_[ACTCLIP3ON] = QObject::tr("Clip3 ON");
    clipGroupMap_[ACTCLIP4ON] = QObject::tr("Clip4 ON");
    clipGroupMap_[ACTCLIP5ON] = QObject::tr("Sucker1 ON");
    clipGroupMap_[ACTCLIP6ON] = QObject::tr("Sucker2 ON");
    clipGroupMap_[ACTCLIP7ON] = QObject::tr("Injection ON");
    clipGroupMap_[ACTCLIP8ON] = QObject::tr("Conveyor ON");
    clipGroupMap_[ACTCLSMDON] = QObject::tr("Lock Mold ON");
    clipGroupMap_[ACTEJECTON] = QObject::tr("Eject ON");
    clipGroupMap_[ACTLAYOUTON] = QObject::tr("Layout ON");
    clipGroupMap_[ACTCLIP12ON] = QObject::tr("Core1 Permit");
    clipGroupMap_[ACTCLIP13ON] = QObject::tr("Core2 Permit");
//    clipGroupMap_[ACTCLIP14ON] = QObject::tr("Clip14 ON");
//    clipGroupMap_[ACTCLIP15ON] = QObject::tr("Clip15 ON");
//    clipGroupMap_[ACTCLIP16ON] = QObject::tr("Clip16 ON");
    clipGroupMap_[ACTCLIP1OFF] = QObject::tr("Clip1 OFF");
    clipGroupMap_[ACTCLIP2OFF] = QObject::tr("Clip2 OFF");
    clipGroupMap_[ACTCLIP3OFF] = QObject::tr("Clip3 OFF");
    clipGroupMap_[ACTCLIP4OFF] = QObject::tr("Clip4 OFF");
    clipGroupMap_[ACTCLIP5OFF] = QObject::tr("Sucker1 OFF");
    clipGroupMap_[ACTCLIP6OFF] = QObject::tr("Sucker2 OFF");
    clipGroupMap_[ACTCLIP7OFF] = QObject::tr("Injection OFF");
    clipGroupMap_[ACTCLIP8OFF] = QObject::tr("Conveyor OFF");
    clipGroupMap_[ACTCLSMDOFF] = QObject::tr("Lock Mold OFF");
    clipGroupMap_[ACTEJECTOFF] = QObject::tr("Eject OFF");
    clipGroupMap_[ACTLAYOUTOFF] = QObject::tr("Layout OFF");
    clipGroupMap_[ACTCLIP12OFF] = QObject::tr("Core1 OFF");
    clipGroupMap_[ACTCLIP13OFF] = QObject::tr("Core2 OFF");
    clipGroupMap_[ACT_AUX1] = QObject::tr("Reserve 1");
    clipGroupMap_[ACT_AUX2] = QObject::tr("Reserve 2");
    clipGroupMap_[ACT_AUX3] = QObject::tr("Reserve 3");
    clipGroupMap_[ACT_AUX4] = QObject::tr("Reserve 4");
    clipGroupMap_[ACT_AUX5] = QObject::tr("Reserve 5");
    clipGroupMap_[ACT_AUX6] = QObject::tr("Reserve 6");
//    clipGroupMap_[ACTCLIP14OFF] = QObject::tr("Clip14 OFF");
//    clipGroupMap_[ACTCLIP15OFF] = QObject::tr("Clip15 OFF");
//    clipGroupMap_[ACTCLIP16OFF] = QObject::tr("Clip16 OFF");
    clipGroupMap_[ACTCLIPEND] = QObject::tr("Clip End");
//    clipGroupMap_.insert(ACT_WaitMoldOpened, QObject::tr("Wait Mold Opened"));
}

void ICInstructParam::InitClassesInfo()
{
    xyzStatusList_.append(GX);
    xyzStatusList_.append(GY);
    xyzStatusList_.append(GZ);
    xyzStatusList_.append(GP);
    xyzStatusList_.append(GQ);
    xyzStatusList_.append(GA);
    xyzStatusList_.append(GB);
    xyzStatusList_.append(GC);
//    xyzStatusList_.append(GYZ);
//    xyzStatusList_.append(GZY);
//    xyzStatusList_.append(GZX);
//    xyzStatusList_.append(GXZ);

    clipStatusList_.append(ACTCLIP7ON);
    clipStatusList_.append(ACTCLIP8ON);
}

bool ICInstructParam::IsStackedAction(int action)
{
    if(action == ACTLAYOUTON)
    {
        return true;
    }
    return false;
}

void ICInstructParam::UpdateTranslate()
{
    InstallMoldInfo();
    UpdateHostParam();
}

void ICInstructParam::UpdateHostParam()
{
    commandStrList_.clear();

    commandStrList_ = ContentParam(ICMold::CurrentMold()->MoldContent());

//    emit HostParamChanged();
}

QStringList ICInstructParam::ContentParam(const QList<ICMoldItem>& moldContent)
{
    QString commandStr;
    QStringList commandStrList;
    foreach(const ICMoldItem &moldItem, moldContent)
    {
        commandStr = ConvertCommandStr(moldItem);
        commandStrList.append(commandStr);
    }
    return commandStrList;
}
