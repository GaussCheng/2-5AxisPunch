# -------------------------------------------------
# Project created by QtCreator 2010-11-30T13:47:10
# -------------------------------------------------
TARGET = Multi-axisManipulatorSystem
TEMPLATE = app
QMAKE_CFLAGS += -std=c99
QMAKE_CXX = ccache $${QMAKE_CXX}
QT += sql

DEFINES += IO_C6
#DEFINES += TEACH_PAGE

SK_SIZE = 8

HW_VERSION = "1.3"
SW_VERSION = "6.0.3"
contains(DEFINES, IO_C6){
HW_TYPE = "C6"
}else{
HW_TYPE = "C5"
}
contains(DEFINES, TEACH_PAGE){
HW_TYPE = $${HW_TYPE}_T
}

S_VERSION = CYR$${SK_SIZE}_$${HW_TYPE}_$${SW_VERSION}
VERSTR = '\\"$${S_VERSION}\\"'
DEFINES += SHOW_VERSION=\"$${VERSTR}\"



suffix = Size$${SK_SIZE}
CONFIG(debug, debug|release) {
suffix = $${suffix}_debug
}
else{
suffix = $${suffix}_release
}
DESTDIR = bin_$${suffix}
OBJECTS_DIR = temp_$${suffix}
UI_DIR = temp_$${suffix}
MOC_DIR = temp_$${suffix}
RCC_DIR = temp_$${suffix}


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
    simulateknob.cpp\
    icrecaldialog.cpp \
    icfile.cpp \
    icbackupdialog.cpp \
    version.cpp \
    icalarmdetailsdialog.cpp
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
    icrecaldialog.h \
    icfile.h \
    icbackupdialog.h \
    version.h \
    icalarmdetailsdialog.h


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
equals(SK_SIZE ,5){
message("Define 5")
DEFINES += HC_SK_5
FORMS    += mainframe_5.ui \
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
include (icprogrampage/icprogrampage.pri)

include (vendor/IndustrialSystemFramework/ICUtility/ICUtility.pri)
include (vendor/ICCustomWidgets/icupdatepackmodel/icupdatepackmodel.pri)

RESOURCES += \
    resource.qrc \
    initconfig.qrc

TRANSLATIONS += Multi-axisManipulatorSystem_ch.ts \
    Multi-axisManipulatorSystem_en.ts

OTHER_FILES += \
    sysconfig/alarminfomation-en \
    sysconfig/alarminfomation-ch \
    sysconfig/hintinfomation-ch \
    sysconfig/hintinfomation-en \
    Multi-axisManipulatorSystem_en.ts \
    Multi-axisManipulatorSystem_ch.ts \
    bin_debug/stylesheet/global.qss \
    bin_debug/sysconfig/systemParameter.hc \
    版本日志.txt \
    问题.txt

FORMS += \
    icrecaldialog.ui \
    icbackupdialog.ui \
    icalarmdetailsdialog.ui

message($$DEFINES)


QMAKE_POST_LINK += "cp *.qm $$DESTDIR"
QMAKE_PRE_LINK += "lrelease $${TARGET}.pro"
#message($${UI_DIR})
system("python rename_ui.py $${UI_DIR}")
contains(QMAKE_CXX, g++){
#QMAKE_POST_LINK += "cp *.qm bin_debug"
}else{
#system("python rename_ui.py temp_$${SK_SIZE}")
unix:QMAKE_POST_LINK += " && HCbcrypt.sh -r $$DESTDIR/$$TARGET"
unix:QMAKE_POST_LINK += "&& chmod +x tools/make_target && tools/make_target $$PWD $$DESTDIR" $${S_VERSION}
target.path = /opt/Qt/apps
configsPathBase = tools/Reinstall
translations.path = $${target.path}
translations.files = *.qm
records.path = /opt/Qt/apps/records
records.files += $${configsPathBase}/$${SK_SIZE}records/*
subs.path = /opt/Qt/apps/subs
subs.files += $${configsPathBase}/subs/*
sysconfig.path = /opt/Qt/apps/sysconfig
sysconfig.files += $${configsPathBase}/$${SK_SIZE}sysconfig/*
resource.path = /opt/Qt/apps/resource
resource.files += $${configsPathBase}/$${SK_SIZE}resource/*
stylesheet.path = /opt/Qt/apps/stylesheet
stylesheet.files += $${configsPathBase}/stylesheet/*
others.path = /opt/Qt/apps
others.files += $${configsPathBase}/3-5AxisRobotDatabase
scripts.path = /usr/bin
scripts.files += $${configsPathBase}/$${SK_SIZE}RunApp/*
keymap.path = /home/root
keymap.files =$${configsPathBase}/$${SK_SIZE}-inch-qmap/*
testApp.path = /opt/Qt/apps
testApp.files += $${configsPathBase}/3a8HardwareTest-$${SK_SIZE}-inch
testApp.files += $${configsPathBase}/3A8HardwareTest.en_CH.qm
INSTALLS += target translations records subs sysconfig resource stylesheet others scripts keymap testApp
for(sh, scripts.files){
QMAKE_POST_LINK += " && chmod +x $${sh}"
}
message($$QMAKE_POST_LINK)
}

