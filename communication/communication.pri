INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/icvirtualhost.h \
    $$PWD/iccommunicationcommandbase.h \
    $$PWD/iccommands.h \
    $$PWD/iccommandprocessor.h

SOURCES += $$PWD/icvirtualhost.cpp \
    $$PWD/iccommunicationcommandbase.cpp \
    $$PWD/iccommands.cpp \
    $$PWD/iccommandprocessor.cpp

include (libmodbus/libmodbus.pri)
