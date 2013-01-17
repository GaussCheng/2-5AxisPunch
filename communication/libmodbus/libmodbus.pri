INCLUDEPATH += $$PWD

HEADERS += $$PWD/modbus.h

win32{SOURCES += $$PWD/modbus.cpp}
!win32{SOURCES += $$PWD/modbus.c}
