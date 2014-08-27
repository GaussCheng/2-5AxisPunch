# -------------------------------------------------
# Project created by QtCreator 2010-11-30T13:47:10
# -------------------------------------------------
TARGET = Multi-axisManipulatorSystem
TEMPLATE = app
QMAKE_CFLAGS += -std=c99

OBJECTS_DIR = temp_8
UI_DIR = temp_8
MOC_DIR = temp_8
RCC_DIR = temp_8
DESTDIR = bin
CONFIG(debug, debug|release) {
#    LIBS += -lprofiler
DESTDIR = bin_debug
OBJECTS_DIR = temp_8_d
UI_DIR = temp_8_d
MOC_DIR = temp_8_d
RCC_DIR = temp_8_d
}


win32{INCLUDEPATH += ./}
SOURCES += main.cpp \
    mainframe.cpp \
    icaxispositionlabel.cpp \
    #icalarmdescriptiondialog.cpp \
    icsystemconfig.cpp \
    icparameterssave.cpp \
    icalarmstring.cpp \
    icsplashscreen.cpp \
    icorigindialog.cpp \
    icreturnpage.cpp \
    icscreensaver.cpp \
    icactiondialog.cpp \
    ictimerpool.cpp \
    ichostcomparepage.cpp \
    icbackuputility.cpp \
    ictipswidget.cpp \
    icdataformatchecker.cpp \
    icprogramformatchecker.cpp \
    icconfigformatchecker.cpp \
    simulateknob.cpp \
    icutility.cpp \
    icrecaldialog.cpp
HEADERS += mainframe.h \
    icaxispositionlabel.h \
    #icalarmdescriptiondialog.h \
    icsystemconfig.h \
    icparameterssave.h \
    icalarmstring.h \
    icsplashscreen.h \
    icorigindialog.h \
    icreturnpage.h \
    icscreensaver.h \
    config.h \
    icactiondialog.h \
    ictimerpool.h \
    ichostcomparepage.h \
    icbackuputility.h \
    ictipswidget.h \
    icdataformatchecker.h \
    icprogramformatchecker.h \
    icconfigformatchecker.h \
    simulateknob.h \
    icutility.h \
    icrecaldialog.h

SK_SIZE = 5
AXIS_COUNT = 5
equals(AXIS_COUNT, 5){
DEFINES += HC_AXIS_COUNT_5
}
equals(AXIS_COUNT, 2){
DEFINES += HC_AXIS_COUNT_2
}
#DEFINES += HC_SK_8_SC
equals(SK_SIZE, 8){
message("Define 8")
DEFINES += HC_SK_8
FORMS    += mainframe_8.ui \
    #icalarmdescriptiondialog.ui \
    icorigindialog.ui \
    icreturnpage.ui \
    icscreensaver.ui \
    icactiondialog.ui \
    ichostcomparepage.ui \
    ictipswidget.ui \
    simulateknob.ui
}
equals(SK_SIZE ,5){
message("Define 5")
DEFINES += HC_SK_5
FORMS    += mainframe.ui \
    #icalarmdescriptiondialog.ui \
    icorigindialog.ui \
    icreturnpage.ui \
    icscreensaver.ui \
    icactiondialog.ui \
    ichostcomparepage.ui \
    ictipswidget.ui \
    simulateknob.ui
}

contains(DEFINES, HC_SK_8_SC){
HEADERS += icaxiskeyboard.h
SOURCES += icaxiskeyboard.cpp
FORMS += icaxiskeyboard.ui
}

include (./categorypage/categorypage.pri)
include (custom_widgets/custom_widgets.pri)
include (function_settings/function_settings.pri)
#include (manual_operation/manual_operation.pri)
include (communication/communication.pri)

include (hcmanualoperation/hcmanualoperation.pri)
include (hcsettings/hcsettings.pri)
include (hcinstruction/hcinstruction.pri)
include (ickeyboard/ickeyboard.pri)
include (hcmaintains/hcmaintains.pri)

RESOURCES += \
    resource.qrc \
    initconfig.qrc

TRANSLATIONS += Multi-axisManipulatorSystem_ch.ts \
    Multi-axisManipulatorSystem_en.ts

OTHER_FILES += \
    sysconfig/alarminfomation-en \
    sysconfig/alarminfomation-ch \
    sysconfig/hintinfomation-ch \
    sysconfig/hintinfomation-en

QMAKE_POST_LINK += "cp *.qm $$DESTDIR"
CONFIG(debug, debug|release){
system("python rename_ui.py temp_8_d")
#QMAKE_POST_LINK += "cp *.qm bin_debug"
}else{
system("python rename_ui.py temp_8")
QMAKE_POST_LINK += "&& arm-linux-strip $$DESTDIR/$$TARGET && HCbcrypt.sh -r $$DESTDIR/$$TARGET"
}

FORMS += \
    icrecaldialog.ui

