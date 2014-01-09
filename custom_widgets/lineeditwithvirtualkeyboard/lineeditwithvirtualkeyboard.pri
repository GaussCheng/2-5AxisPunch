INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    virtualkeyboarddialog.h \
    iclineeditwithvirtualkeyboard.h

SOURCES += \
    virtualkeyboarddialog.cpp \
    iclineeditwithvirtualkeyboard.cpp

contains(DEFINES, HC_SK_8){
FORMS += \
    $$PWD/virtualkeyboarddialog_8.ui
}else{
FORMS += \
    $$PWD/virtualkeyboarddialog.ui
}

