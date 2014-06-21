#include "icinstructparam.h"

#include <QObject>

#include "icparameterconversion.h"
#include "icparameterssave.h"
#include "moldinformation.h"
#include "icmacrosubroutine.h"
#include "config.h"
#include "icsystemconfig.h"

#include <QDebug>

ICInstructParam * ICInstructParam::instance_ = NULL;
QMap<int, QString> ICInstructParam::actionGroupMap_;
QMap<int, QString> ICInstructParam::clipGroupMap_;
QList<int> ICInstructParam::xyzStatusList_;
QList<int> ICInstructParam::clipStatusList_;
QMap<int, QString> ICInstructParam::countWayMap_;
QStringList mStr;

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
    if(moldItem.Num() == 0)
    {
        commandStr += tr("Home") + "     "/* + "*" + "    "*/;
    }
    else if(moldItem.SubNum() == 255)
    {
        commandStr += QString::number(moldItem.Num()) + "    " /*+ "*" + "    "*/;
    }
    else
    {
        commandStr += QString::number(moldItem.Num()) + "    " /*+ QString::number(moldItem.SubNum()) + "    "*/;
    }

    uint action;
    action = moldItem.Action();
    if(!actionGroupMap_.contains(action)) return commandStr;
    commandStr += actionGroupMap_.value(action) + ": ";

    if(xyzStatusList_.contains(action))
    {
        //            commandStr += ICParameterConversion::TransThisIntToThisText(moldItem.Pos(), 1) + " ";
        if(action == ICMold::GARC)
        {
            commandStr.chop(2);
            commandStr += "-";
            commandStr += actionGroupMap_.value(moldItem.IFPos()) + ":";
        }
        if(moldItem.IsBadProduct() == 1)
        {
            commandStr += ICUserDefineConfig::Instance()->GetPointsLocaleName(moldItem.SubNum(), "zh");
        }
        else
        {
            commandStr += ICParameterConversion::TransThisIntToThisText(moldItem.ActualPos(), POS_DECIMAL) + " ";
        }
        //            commandStr += "X" + ICParameterConversion::TransThisIntToThisText(moldItem.X(), 1) + " ";
        //            commandStr += "Y" + ICParameterConversion::TransThisIntToThisText(moldItem.Y(), 1) + " ";
        //            commandStr += "Z" + ICParameterConversion::TransThisIntToThisText(moldItem.Z(), 1) + " ";
        //            if(action != G01)
        //            {
        //                commandStr == QObject::tr("Radius:") + ICParameterConversion::TransThisIntToThisText(moldItem.RVal(), 1);
        //            }
        commandStr += tr("Speed:") + QString::number(moldItem.SVal()) + " ";
        if(moldItem.IsEarlyEnd())
        {
            if(!moldItem.IsEarlySpeedDown())
            {
                commandStr += tr("Early End,Early Position:");
                //                    commandStr += QString().sprintf("%.1f", moldItem.IFPos() / (qreal)10) + " ";
                commandStr += QString::number(moldItem.ActualIfPos()) + " ";
            }

        }
        if(moldItem.IsEarlySpeedDown())
        {
            if(!moldItem.IsEarlyEnd())
            {
                commandStr += tr("Early Speed-Down:") + QString::number(moldItem.GetEarlyDownSpeed()) + " ";
                //                    commandStr += tr("Early Position:") + QString().sprintf("%.1f", moldItem.IFPos() / (qreal)10) + " ";
                commandStr += tr("Early Position:") + QString::number(moldItem.ActualIfPos()) + " ";
            }
            else
            {
                commandStr += tr("Early End,");
                commandStr += tr("Early Speed-Down:") + QString::number(moldItem.GetEarlyDownSpeed()) + " ";
                //                    commandStr += tr("Early Position:") + QString().sprintf("%.1f", moldItem.IFPos() / (qreal)10) + " ";
                commandStr += tr("Early Position:") + QString::number(moldItem.ActualIfPos()) + " ";
            }
        }


//        if(moldItem.IsBadProduct() && moldItem.Action() == ICMold::GZ)
//        {
//            commandStr += QObject::tr("Bad Product En") + " ";
//        }
//        commandStr += tr("Delay") + ":" + ICParameterConversion::TransThisIntToThisText(moldItem.DVal(), 2);
//        return commandStr;
    }
    else
    {
        ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
        if(action == ICMold::GCheckX || action == ICMold::GCondition)
        {
            if(moldItem.IFVal() & 0x80)
            {
                commandStr += QString("M%1 ").arg(QString::number(moldItem.SubNum() + 8, 8));
                if(moldItem.SubNum() < mStr.size())
                {
                    commandStr += mStr.at(moldItem.SubNum());
                }
            }
            else
            {
                ICUserIOInfo info = config->XInfo(moldItem.SubNum());
                commandStr += info.code + ":" + info.GetLocaleName("zh") + " ";
            }
            commandStr += ((moldItem.IFVal() & 0x7F) == 0) ? "OFF" :"ON ";
            if(action == ICMold::GCondition)
            {
                commandStr += QObject::tr("Return:") + QString::number((int)moldItem.SVal());
            }
            commandStr += QObject::tr("Limit") + ":" + ICParameterConversion::TransThisIntToThisText(moldItem.DVal(), 2);
            return commandStr;

        }
        else if(action == ICMold::GWait)
        {
//            ICUserIOInfo info = config->EuXinfo(moldItem.SubNum());
//            commandStr +=  info.GetLocaleName("zh") + " ";
            commandStr += config->EuXString(moldItem.SubNum());
            commandStr += (moldItem.IFVal() == 0) ? "OFF" :"ON ";
            commandStr += QObject::tr("Limit") + ":" + ICParameterConversion::TransThisIntToThisText(moldItem.DVal(), 2);
            return commandStr;
        }
        else if(action == ICMold::GEuOut)
        {
//            ICUserIOInfo info = config->EuYinfo(moldItem.SubNum());
//            commandStr +=  info.GetLocaleName("zh") + " ";
            commandStr += config->EuYString(moldItem.SubNum());
        }
        else if(action == ICMold::GMWait)
        {

            commandStr += QString("M%1 ").arg(QString::number(moldItem.SubNum() + 8, 8));
            if(moldItem.SubNum() < mStr.size())
            {
                commandStr += mStr.at(moldItem.SubNum());
            }
            commandStr += (moldItem.IFVal() == 0) ? "OFF" :"ON ";
            commandStr += QObject::tr("Limit") + ":" + ICParameterConversion::TransThisIntToThisText(moldItem.DVal(), 2);
            return commandStr;

        }
        else if(action == ICMold::GMOut)
        {
            commandStr += QString("M%1 ").arg(QString::number(moldItem.SubNum() + 8, 8));
            if(moldItem.SubNum() < mStr.size())
            {
                commandStr += mStr.at(moldItem.SubNum());
            }
//            commandStr += (moldItem.IFVal() == 0) ? "OFF" :"ON ";
//            commandStr += QObject::tr("Limit") + ":" + ICParameterConversion::TransThisIntToThisText(moldItem.DVal(), 2);
//            return commandStr;
        }
        else if(action == ICMold::GStack)
        {
            commandStr += QString(QObject::tr("Group%1")).arg(moldItem.SVal());
        }
        else/* if(action == ICMold::GOutY)*/
        {
            ICUserIOInfo info;
            if(action == ICMold::GTwoXTwoY)
            {
                info = config->YInfo(moldItem.SubNum() << 1);
            }
            else
            {
                info = config->YInfo(moldItem.SubNum());
            }
            commandStr += info.code + ":" + info.GetLocaleName("zh") + " ";
//            commandStr += config->GetIOActionLocaleName(action  - ICMold::GOutY, moldItem.SubNum(), moldItem.IFVal(), "zh") + " ";
        }
        if(action != ICMold::ACTEND && action != ICMold::GStack)
        {
            commandStr += (moldItem.IFVal() == 0) ? "OFF" :"ON ";
        }
        commandStr += " ";
    }
    commandStr += QObject::tr("Delay") + ":" + ICParameterConversion::TransThisIntToThisText(moldItem.DVal(), 2);
    return commandStr;
}

void ICInstructParam::AppendCommandStrList(const QString & commandStr)
{
    commandStrList_.append(commandStr);;
}

void ICInstructParam::InstallMoldInfo()
{
    actionGroupMap_[ICMold::GC] = QObject::tr("C");
    actionGroupMap_[ICMold::GX] = QObject::tr("X1");
    actionGroupMap_[ICMold::GY] = QObject::tr("Y1");
    actionGroupMap_[ICMold::GZ] = QObject::tr("Z");
    actionGroupMap_[ICMold::GP] = QObject::tr("X2");
    actionGroupMap_[ICMold::GQ] = QObject::tr("Y2");
    actionGroupMap_[ICMold::GA] = QObject::tr("A");
    actionGroupMap_[ICMold::GB] = QObject::tr("B");
    actionGroupMap_[ICMold::GOutY] = QObject::tr("OutY");
    actionGroupMap_.insert(ICMold::GCheckX, QObject::tr("Wait X"));
    actionGroupMap_.insert(ICMold::GOneXOneY, QObject::tr("One X One Y"));
    actionGroupMap_.insert(ICMold::GTwoXTwoY, QObject::tr("Two X Two Y"));
    actionGroupMap_.insert(ICMold::GTo, QObject::tr("Move to"));
    actionGroupMap_.insert(ICMold::GWait, QObject::tr("Wait EuX"));
    actionGroupMap_.insert(ICMold::GEuOut, QObject::tr("Eu out"));
    actionGroupMap_.insert(ICMold::GCondition, QObject::tr("Check X"));
    actionGroupMap_.insert(ICMold::GMWait, QObject::tr("Wait M"));
    actionGroupMap_.insert(ICMold::GMOut, QObject::tr("M out"));
    actionGroupMap_.insert(ICMold::GStack, QObject::tr("Stack"));
    actionGroupMap_.insert(ICMold::GARC, QObject::tr("3-D"));
//    actionGroupMap_[ACTMAINUP] = QObject::tr("Main arm up");
//    actionGroupMap_[ACTMAINDOWN] = QObject::tr("Main arm down");
//    actionGroupMap_[ACTMAINFORWARD] = QObject::tr("Main arm forward");
//    actionGroupMap_[ACTMAINBACKWARD] = QObject::tr("Main arm backward");
//    actionGroupMap_[ACTPOSEHORI] = QObject::tr("Horizontal posture 1");
//    actionGroupMap_[ACTPOSEVERT] = QObject::tr("Vertical posture 1");
//    actionGroupMap_[ACT_PoseHori2] = QObject::tr("Horizontal posture 2");
//    actionGroupMap_[ACT_PoseVert2] = QObject::tr("Vertical posture 2");
//    actionGroupMap_[ACTVICEUP] = QObject::tr("Vice arm up");
//    actionGroupMap_[ACTVICEDOWN] = QObject::tr("Vice arm down");
//    actionGroupMap_[ACTVICEFORWARD] = QObject::tr("Vice arm forward");
//    actionGroupMap_[ACTVICEBACKWARD] = QObject::tr("Vice arm backward");
//    actionGroupMap_[ACTGOOUT] = QObject::tr("Traverse out");
//    actionGroupMap_[ACTCOMEIN] = QObject::tr("Traverse in");

    //    actionGroupMap_.insert(ACTMAINMIDDOWN, QObject::tr("Main mid down"));
//    actionGroupMap_.insert(ACTCHECKINPUT, QObject::tr("Check"));
    actionGroupMap_.insert(ICMold::ACTEND, QObject::tr("Mold end"));
//    actionGroupMap_.insert(ACTParallel, QObject::tr("Parallel"));
//    actionGroupMap_.insert(ACT_WaitMoldOpened, QObject::tr("Wait"));
//    actionGroupMap_.insert(ACT_Cut, QObject::tr("Cut"));
//    actionGroupMap_.insert(ACT_OTHER, QObject::tr("Other"));

//    clipGroupMap_[ACTCLIP1ON] = QObject::tr("Clip1 ON");
//    clipGroupMap_[ACTCLIP2ON] = QObject::tr("Clip2 ON");
//    clipGroupMap_[ACTCLIP3ON] = QObject::tr("Clip3 ON");
//    clipGroupMap_[ACTCLIP4ON] = QObject::tr("Clip4 ON");
//    clipGroupMap_[ACTCLIP5ON] = QObject::tr("Sucker1 ON");
//    clipGroupMap_[ACTCLIP6ON] = QObject::tr("Sucker2 ON");
//    clipGroupMap_[ACTCLIP7ON] = QObject::tr("Injection ON");
//    clipGroupMap_[ACTCLIP8ON] = QObject::tr("Conveyor ON");
//    clipGroupMap_[ACTCLSMDON] = QObject::tr("Lock Mold ON");
//    //    clipGroupMap_[ACTEJECTON] = QObject::tr("Eject ON 1");
//    clipGroupMap_[ACTEJECTON] = QObject::tr("Eject ON");
//    clipGroupMap_[ACTLAYOUTON] = QObject::tr("Layout ON");
//    //    clipGroupMap_[ACTCLIP12ON] = QObject::tr("Lock 2 Mold ON");
//    clipGroupMap_[ACTCLIP12ON] = QObject::tr("Core1 Permit");
//    clipGroupMap_[ACTCLIP13ON] = QObject::tr("Core2 Permit");
//    //    clipGroupMap_[ACTCLIP14ON] = QObject::tr("Clip14 ON");
//    //    clipGroupMap_[ACTCLIP15ON] = QObject::tr("Clip15 ON");
//    //    clipGroupMap_[ACTCLIP16ON] = QObject::tr("Clip16 ON");
//    clipGroupMap_[ACTCLIP1OFF] = QObject::tr("Clip1 OFF");
//    clipGroupMap_[ACTCLIP2OFF] = QObject::tr("Clip2 OFF");
//    clipGroupMap_[ACTCLIP3OFF] = QObject::tr("Clip3 OFF");
//    clipGroupMap_[ACTCLIP4OFF] = QObject::tr("Clip4 OFF");
//    clipGroupMap_[ACTCLIP5OFF] = QObject::tr("Sucker1 OFF");
//    clipGroupMap_[ACTCLIP6OFF] = QObject::tr("Sucker2 OFF");
//    clipGroupMap_[ACTCLIP7OFF] = QObject::tr("Injection OFF");
//    clipGroupMap_[ACTCLIP8OFF] = QObject::tr("Conveyor OFF");
//    clipGroupMap_[ACTCLSMDOFF] = QObject::tr("Lock Mold OFF");
//    //    clipGroupMap_[ACTEJECTOFF] = QObject::tr("Eject OFF 1");
//    clipGroupMap_[ACTEJECTOFF] = QObject::tr("Eject OFF");
//    clipGroupMap_[ACTLAYOUTOFF] = QObject::tr("Layout OFF");
//    //    clipGroupMap_[ACTCLIP12OFF] = QObject::tr("Lock 2 Mold OFF");
//    clipGroupMap_[ACTCLIP12OFF] = QObject::tr("Core1 OFF");
//    clipGroupMap_[ACTCLIP13OFF] = QObject::tr("Core2 OFF");
//    clipGroupMap_[ACT_AUX1] = QObject::tr("Reserve 1");
//    //    clipGroupMap_[ACT_AUX1] = QObject::tr("");
//    clipGroupMap_[ACT_AUX2] = QObject::tr("Reserve 2");
//    clipGroupMap_[ACT_AUX3] = QObject::tr("Reserve 3");
//    clipGroupMap_[ACT_AUX4] = QObject::tr("Reserve 4");
//    clipGroupMap_[ACT_AUX5] = QObject::tr("Reserve 5");
//    clipGroupMap_[ACT_AUX6] = QObject::tr("Reserve 6");
//    //    clipGroupMap_[ACTCLIP14OFF] = QObject::tr("Clip14 OFF");
//    //    clipGroupMap_[ACTCLIP15OFF] = QObject::tr("Clip15 OFF");
//    //    clipGroupMap_[ACTCLIP16OFF] = QObject::tr("Clip16 OFF");
//    clipGroupMap_[ACTCLIPEND] = QObject::tr("Clip End");
    //    clipGroupMap_.insert(ACT_WaitMoldOpened, QObject::tr("Wait Mold Opened"));
    mStr<<tr("M10")<<tr("M11")<<tr("M12")<<tr("M13");
}

void ICInstructParam::InitClassesInfo()
{
    xyzStatusList_.append(ICMold::GX);
    xyzStatusList_.append(ICMold::GY);
    xyzStatusList_.append(ICMold::GZ);
    xyzStatusList_.append(ICMold::GP);
    xyzStatusList_.append(ICMold::GQ);
    xyzStatusList_.append(ICMold::GA);
    xyzStatusList_.append(ICMold::GB);
    xyzStatusList_.append(ICMold::GC);
    xyzStatusList_.append(ICMold::GTo);
    xyzStatusList_.append(ICMold::GARC);
    //    xyzStatusList_.append(GYZ);
    //    xyzStatusList_.append(GZY);
    //    xyzStatusList_.append(GZX);
    //    xyzStatusList_.append(GXZ);

//    clipStatusList_.append(ACTCLIP7ON);
//    clipStatusList_.append(ACTCLIP8ON);
//    clipStatusList_.append(ACTCLIP7OFF);
//    clipStatusList_.append(ACTCLIP8OFF);

//    countWayMap_.insert(0, tr("All"));
//    countWayMap_.insert(1, tr("Good"));
//    countWayMap_.insert(2, tr("Stacked-1"));
//    countWayMap_.insert(3, tr("Stacked-2"));
//    countWayMap_.insert(4, tr("Stacked-3"));
//    countWayMap_.insert(5, tr("Stacked-4"));
    //    clipStatusList_.append(ACT_AUX1);
    //    clipStatusList_.append(ACT_AUX2);
    //    clipStatusList_.append(ACT_AUX3);
    //    clipStatusList_.append(ACT_AUX4);
    //    clipStatusList_.append(ACT_AUX5);
    //    clipStatusList_.append(ACT_AUX6);
}

bool ICInstructParam::IsStackedAction(int action)
{
//    if(action == ACTLAYOUTON)
//    {
//        return true;
//    }
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
