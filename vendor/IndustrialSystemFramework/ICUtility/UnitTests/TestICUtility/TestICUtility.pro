#-------------------------------------------------
#
# Project created by QtCreator 2011-09-06T10:42:25
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testicutility
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS +=-L../../ -lICUtility


SOURCES += tst_testicutility.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
