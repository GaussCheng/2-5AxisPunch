INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/virtualkeyboarddialog.h \
    $$PWD/iclineeditwithvirtualkeyboard.h

SOURCES += \
    $$PWD/virtualkeyboarddialog.cpp \
    $$PWD/iclineeditwithvirtualkeyboard.cpp

contains(DEFINES, HC_SK_8){
FORMS += \
    $$PWD/virtualkeyboarddialog_8.ui
}else{
FORMS += \
    $$PWD/virtualkeyboarddialog.ui
}

