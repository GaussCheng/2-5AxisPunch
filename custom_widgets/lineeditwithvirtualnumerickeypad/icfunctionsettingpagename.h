#ifndef ICFUNCTIONSETTINGPAGENAME_H
#define ICFUNCTIONSETTINGPAGENAME_H

#include <QString>

class ICFunctionSettingPageName
{
public:
    static ICFunctionSettingPageName* Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICFunctionSettingPageName();
        }
        return instance_;
    }

public:
    const QString SignalSettingPageName;
    const QString ProductSettingPageName;
    const QString SpecialSettingPageName;
    const QString AuxiliaryMachinePageName;
    const QString InstructionPageName;
    const QString SecurityPointSettingPageName;
    const QString StandbySettingWidgetPageName;
    const QString StackSettingPageName;
    const QString PlacementSettingPageName;
    const QString FixedProgramPageName;
    const QString SystemSettingPageName;

private:
    static ICFunctionSettingPageName* instance_;
    ICFunctionSettingPageName();
};

#endif // ICFUNCTIONSETTINGPAGENAME_H
