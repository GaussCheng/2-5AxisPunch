#-------------------------------------------------
#
# Project created by QtCreator 2011-07-22T09:45:09
#
#-------------------------------------------------

QT       -= gui
QT       += network sql
TARGET = ICCore
TEMPLATE = lib
CONFIG += warn_on
DEFINES += ICCORE_LIBRARY
VERSION = 0.1.0
CONFIG(debug, debug|release) {
    DESTDIR = ../libs_debug
    unix:LIBS += -L../libs_debug -lICUtility
    win32:LIBS += -L../libs_debug -lICUtility0
    OBJECTS_DIR = temp_debug
    UI_DIR = temp_debug
    MOC_DIR = temp_debug
    RCC_DIR = temp_debug
} else {
    DESTDIR = ../libs
    unix:LIBS += -L../libs -lICUtility
    win32:LIBS += -L../libs -lICUtility0
    OBJECTS_DIR = temp_release
    UI_DIR = temp_release
    MOC_DIR = temp_release
    RCC_DIR = temp_release
}

include("ICPeripherals/icperipherals.pri")
include("ICCommunication/iccommunication.pri")
include("ICLogSystem/iclogsystem.pri")
include("ICLevelManager/ICLevelManager.pri")
INCLUDEPATH += ../include

SOURCES += \
    icaddrwrapper.cpp \
    icparameterscache.cpp

HEADERS +=\
        ICCore_global.h \
    icaddrwrapper.h \
    icparameterscache.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE08FB7ED
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ICCore.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
