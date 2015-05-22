#-------------------------------------------------
#
# Project created by QtCreator 2011-07-22T15:04:47
#
#-------------------------------------------------

QT       -= gui

TARGET = ICUtility
TEMPLATE = lib
VERSION = 0.1.0
CONFIG(debug, debug|release) {
    DESTDIR = ../libs_debug
    OBJECTS_DIR = temp_debug
    UI_DIR = temp_debug
    MOC_DIR = temp_debug
    RCC_DIR = temp_debug
} else {
    DESTDIR = ../libs
    OBJECTS_DIR = temp_release
    UI_DIR = temp_release
    MOC_DIR = temp_release
    RCC_DIR = temp_release
}

DEFINES += ICUTILITY_LIBRARY

SOURCES += icutility.cpp \
    ictimerpool.cpp \
    icupdatesystem.cpp \
    icobjectpool.cpp \
    icflagspool.cpp \
    iccyclearray.cpp \
    icpic32uartbootloaderprotocol.cpp \
    icalarmclock.cpp \
    icregister.cpp \
    iccommonappsettings.cpp

HEADERS += icutility.h\
        ICUtility_global.h \
    ictimerpool.h \
    containerinit.h \
    icoptimize.h \
    icupdatesystem.h \
    icobjectpool.h \
    icflagspool.h \
    iccyclearray.h \
    icpic32uartbootloaderprotocol.h \
    icalarmclock.h \
    icregister.h \
    iccommonappsettings.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE0E7A9A7
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ICUtility.dll
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
