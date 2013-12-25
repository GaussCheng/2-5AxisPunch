INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    function_settings/moldinformation.h
    #function_settings/icstacksettingpageframe.h \
    #function_settings/icstandbysettingframe.h \
    #function_settings/icsystemsettingframe.h \
    #function_settings/icproductsettingframe.h \
    #function_settings/icsignalsettingframe.h \
    #function_settings/icsecuritypointsettingframe.h \
    #function_settings/icservosettingframe.h \
#    function_settings/servospeedsettingframe.h \
#    function_settings/servooperatingparamsettingframe.h \
#    function_settings/pneumaticaxissettingframe.h \
    #function_settings/icplacementsettingframe.h \
#    function_settings/programmonitorframe.h \
#    function_settings/programeditorframe.h
#    function_settings/placementfinishedframe.h \
#    function_settings/placementdefectiveproductsframe.h \
#    function_settings/placementqualitycontrolframe.h \
#    function_settings/placementweighframe.h \
#    function_settings/placementmaterialhandlepositionframe.h \
#    function_settings/placementotherframe.h

SOURCES += \
    function_settings/moldinformation.cpp
    #function_settings/icstacksettingpageframe.cpp \
    #function_settings/icstandbysettingframe.cpp \
    #function_settings/icsystemsettingframe.cpp \
    #function_settings/icproductsettingframe.cpp \
    #function_settings/icsignalsettingframe.cpp \
    #function_settings/icsecuritypointsettingframe.cpp \
    #function_settings/icservosettingframe.cpp \
#    function_settings/servospeedsettingframe.cpp \
#    function_settings/servooperatingparamsettingframe.cpp \
#    function_settings/pneumaticaxissettingframe.cpp \
    #function_settings/icplacementsettingframe.cpp \
#    function_settings/programmonitorframe.cpp \
#    function_settings/programeditorframe.cpp
#    function_settings/placementfinishedframe.cpp \
#    function_settings/placementdefectiveproductsframe.cpp \
#    function_settings/placementqualitycontrolframe.cpp \
#    function_settings/placementweighframe.cpp \
#    function_settings/placementmaterialhandlepositionframe.cpp \
#    function_settings/placementotherframe.cpp

contains(DEFINES, HC_SK_5){
FORMS += \
    function_settings/moldinformation.ui
    #function_settings/icstacksettingpageframe.ui \
    #function_settings/icstandbysettingframe.ui \
    #function_settings/icsystemsettingframe.ui \
    #function_settings/icproductsettingframe.ui \
    #function_settings/icsignalsettingframe.ui \
    #function_settings/icsecuritypointsettingframe.ui \
    #function_settings/icservosettingframe.ui \
#    function_settings/servospeedsettingframe.ui \
#    function_settings/servooperatingparamsettingframe.ui \
#    function_settings/pneumaticaxissettingframe.ui \
    #function_settings/icplacementsettingframe.ui \
#    function_settings/programmonitorframe.ui \
#    function_settings/programeditorframe.ui
#    function_settings/placementfinishedframe.ui \
#    function_settings/placementdefectiveproductsframe.ui \
#    function_settings/placementqualitycontrolframe.ui \
#    function_settings/placementweighframe.ui \
#    function_settings/placementmaterialhandlepositionframe.ui \
#    function_settings/placementotherframe.ui
}

contains(DEFINES, HC_SK_8){
FORMS += \
    function_settings/moldinformation_8.ui
}
