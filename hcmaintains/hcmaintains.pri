INCLUDEPATH += $$PWD

HEADERS += \
    hcmaintains/icupdatelogodialog.h \
    hcmaintains/icupdatesystempage.h

SOURCES += \
    hcmaintains/icupdatelogodialog.cpp \
    hcmaintains/icupdatesystempage.cpp

FORMS += \
#    hcmaintains/icupdatesystem.ui \
    hcmaintains/icupdatelogodialog.ui

contains(DEFINES, HC_SK_8){
FORMS += hcmaintains/icupdatesystempage_8.ui
}
contains(DEFINES, HC_SK_5){
FORMS += hcmaintains/icupdatesystempage.ui
}
