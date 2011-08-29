INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += icvirtualhost.h \
    iccommunicationcommandbase.h \
    iccommands.h \
    iccommandprocessor.h

SOURCES += icvirtualhost.cpp \
    iccommunicationcommandbase.cpp \
    iccommands.cpp \
    iccommandprocessor.cpp

include (libmodbus/libmodbus.pri)
