INCLUDEPATH += $$PWD

HEADERS += \
    hcmanualoperation/ichcmanualoperationpageframe.h \
    hcmanualoperation/axismodifydialog.h
    #hcmanualoperation/hcmanualalternateioframe.h \
    #hcmanualoperation/hcpneumaticaxiscontrolframe.h \
    #hcmanualoperation/hcmanualfunctionframe.h \

SOURCES += \
    hcmanualoperation/ichcmanualoperationpageframe.cpp \
    hcmanualoperation/axismodifydialog.cpp
    #hcmanualoperation/hcmanualalternateioframe.cpp \
    #hcmanualoperation/hcpneumaticaxiscontrolframe.cpp \
    #hcmanualoperation/hcmanualfunctionframe.cpp \

contains(DEFINES, HC_SK_8){
FORMS += \
    hcmanualoperation/ichcmanualoperationpageframe.ui \
    hcmanualoperation/axismodifydialog.ui
    #hcmanualoperation/hcmanualalternateioframe.ui \
    #hcmanualoperation/hcpneumaticaxiscontrolframe.ui \
    #hcmanualoperation/hcmanualfunctionframe.ui \
}
contains(DEFINES, HC_SK_5){
FORMS += \
    hcmanualoperation/ichcmanualoperationpageframe_5.ui \
    hcmanualoperation/axismodifydialog_5.ui
    #hcmanualoperation/hcmanualalternateioframe.ui \
    #hcmanualoperation/hcpneumaticaxiscontrolframe.ui \
    #hcmanualoperation/hcmanualfunctionframe.ui \
}
