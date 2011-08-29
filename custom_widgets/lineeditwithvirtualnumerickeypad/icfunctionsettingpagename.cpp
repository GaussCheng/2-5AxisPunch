#include "icfunctionsettingpagename.h"

ICFunctionSettingPageName * ICFunctionSettingPageName::instance_;

ICFunctionSettingPageName::ICFunctionSettingPageName()
    : SignalSettingPageName("SignalSettingPage"),
    ProductSettingPageName("ProductSettingPage"),
    SpecialSettingPageName("SpecialSettingPage"),
    AuxiliaryMachinePageName("AuxiliaryMachinePage"),
    InstructionPageName("InstructionPage"),
    SecurityPointSettingPageName("SecurityPointSettingPage"),
    StandbySettingWidgetPageName("StandbySettingWidgetPage"),
    StackSettingPageName("StackSettingPage"),
    PlacementSettingPageName("PlacementSettingPage"),
    FixedProgramPageName("FixedProgramPage"),
    SystemSettingPageName("SystemSettingPage")
{
}
