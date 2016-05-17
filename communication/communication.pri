INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/icvirtualhost.h \
    $$PWD/iccommunicationcommandbase.h \
    $$PWD/iccommands.h \
    $$PWD/iccommandprocessor.h \
    communication/icnwm.h \
    communication/icnwmprotocol.h

SOURCES += $$PWD/icvirtualhost.cpp \
    $$PWD/iccommunicationcommandbase.cpp \
    $$PWD/iccommands.cpp \
    $$PWD/iccommandprocessor.cpp \
    communication/icnwm.cpp \
    communication/icnwmprotocol.cpp

include (libmodbus/libmodbus.pri)
