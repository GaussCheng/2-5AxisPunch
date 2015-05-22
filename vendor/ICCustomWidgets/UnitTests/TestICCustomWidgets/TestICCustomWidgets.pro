#-------------------------------------------------
#
# Project created by QtCreator 2012-03-14T16:37:28
#
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_testiccustomwidgets
CONFIG   += console
CONFIG   -= app_bundle

OBJECTS_DIR = temp
UI_DIR = temp
MOC_DIR = temp
RCC_DIR = temp

TEMPLATE = app


INCLUDEPATH += /usr/local/qwt-6.0.2-svn/include
SOURCES += tst_testiccustomwidgets.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
include (../../iccustomwidgets.pri)

HEADERS += \
    tst_testiccustomwidgets.h

LIBS += -L/usr/local/qwt-6.0.2-svn/lib -lqwt
