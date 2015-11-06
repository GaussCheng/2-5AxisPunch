INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/virtualnumerickeypaddialog.h \
    $$PWD/icparameterconversion.h \
    $$PWD/iclineeditwithvirtualnumerickeypad.h \
    $$PWD/icfunctionsettingpagename.h

SOURCES += \
    $$PWD/virtualnumerickeypaddialog.cpp \
    $$PWD/icparameterconversion.cpp \
    $$PWD/iclineeditwithvirtualnumerickeypad.cpp \
    $$PWD/icfunctionsettingpagename.cpp

#FORMS +=
#    custom_widgets/lineeditwithvirtualnumerickeypad/virtualnumerickeypaddialog.ui \
#    custom_widgets/lineeditwithvirtualnumerickeypad/virtualnumerickeypaddialog_5.ui

RESOURCES += \
    resource.qrc
contains(DEFINES, HC_SK_8){
FORMS += $$PWD/virtualnumerickeypaddialog.ui
}
contains(DEFINES, HC_SK_5){
FORMS += $$PWD/virtualnumerickeypaddialog_5.ui
}
