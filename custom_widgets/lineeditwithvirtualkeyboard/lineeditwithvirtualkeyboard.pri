INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/virtualkeyboarddialog.h \
    $$PWD/iclineeditwithvirtualkeyboard.h

SOURCES += \
    $$PWD/virtualkeyboarddialog.cpp \
    $$PWD/iclineeditwithvirtualkeyboard.cpp

#FORMS +=
#    virtualkeyboarddialog.ui

contains(DEFINES, HC_SK_8){
FORMS += $$PWD/virtualkeyboarddialog.ui
}
contains(DEFINES, HC_SK_5){
FORMS += $$PWD/virtualkeyboarddialog_5.ui
}
