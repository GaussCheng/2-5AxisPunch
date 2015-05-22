#-------------------------------------------------
#
# Project created by QtCreator 2012-03-28T14:45:08
#
#-------------------------------------------------

QT       += sql gui
TARGET = ICCustomWidgets
TEMPLATE = lib
OBJECTS_DIR = temp
UI_DIR = temp
MOC_DIR = temp
RCC_DIR = temp

DEFINES += ICCUSTOMWIDGETS_LIBRARY

CONFIG(debug, debug|release) {
    #INCLUDEPATH += /usr/local/qwt-6.0.2-svn/include
    INCLUDEPATH += qwt-6.0/src
    DESTDIR = libs_debug
#    LIBS += -L./qwt-6.0/libs_debug -lqwt
} else {
    INCLUDEPATH += ./include/
    DESTDIR = libs
    unix:LIBS += -L./qwt-6.0/libs -lqwt
    win32:LIBS += -L./qwt-6.0/libs -lqwt
    OBJECTS_DIR = temp_armv6_share_op
    UI_DIR = temp_armv6_share_op
    MOC_DIR = temp_armv6_share_op
    RCC_DIR = temp_armv6_share_op
}

include(iccustomwidgets.pri)
SOURCES +=

HEADERS +=\
        ICCustomWidgets_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7CB3E50
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ICCustomWidgets.dll
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
TRANSLATIONS += ICCustomWidgets_zh_CN.ts \
    ICCustomWidgets_en_US.ts
system(python tools/CPHeader.py . include)
