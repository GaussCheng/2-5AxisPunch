INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    custom_widgets/programhead/icprogramheadframe.h \
    custom_widgets/programhead/passwdlevellabel.h \
    custom_widgets/programhead/passworddialog.h

SOURCES += \
    custom_widgets/programhead/icprogramheadframe.cpp \
    custom_widgets/programhead/passwdlevellabel.cpp \
    custom_widgets/programhead/passworddialog.cpp

contains(DEFINES, HC_SK_8){
FORMS += \
    custom_widgets/programhead/icprogramheadframe_8.ui \
    custom_widgets/programhead/passworddialog.ui
}

contains(DEFINES, HC_SK_5){
FORMS += \
    custom_widgets/programhead/icprogramheadframe.ui \
    custom_widgets/programhead/passworddialog.ui
}
