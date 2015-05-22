#-------------------------------------------------
#
# Project created by QtCreator 2011-07-22T14:35:30
#
#-------------------------------------------------

QT       += core gui

TARGET = ICScreenSaverGUI
TEMPLATE = app
DESTDIR = ../../bin
INCLUDEPATH += ../../include

CONFIG(debug, debug|release) {
    LIBS += -L../../libs_debug -lICUtility
} else {
    LIBS += -L../../libs_release -lICUtility
}

SOURCES += main.cpp\
        icscreensavergui.cpp

HEADERS  += icscreensavergui.h
