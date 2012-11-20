#-------------------------------------------------
#
# Project created by QtCreator 2012-09-25T18:02:12
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testicaxismanipulatorsystem
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../

SOURCES += tst_testicaxismanipulatorsystem.cpp \
    ../../icbackuputility.cpp \
    ../../icdataformatchecker.cpp \
    ../../icprogramformatchecker.cpp \
    ../../icconfigformatchecker.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../icbackuputility.h \
    ../../icdataformatchecker.h \
    ../../icprogramformatchecker.h \
    ../../icconfigformatchecker.h
