INCLUDEPATH += $$PWD

HEADERS += $$PWD/modbus.h

win32{
    SOURCES += $$PWD/modbus.cpp
}
unix{
    SOURCES += $$PWD/modbus.c

}
