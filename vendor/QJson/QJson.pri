INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/serializerrunnable.h \
    $$PWD/serializer.h \
    $$PWD/qobjecthelper.h \
    $$PWD/qjson_export.h \
    $$PWD/qjson_debug.h \
    $$PWD/position.hh \
    $$PWD/parserrunnable.h \
    $$PWD/parser_p.h \
    $$PWD/parser.h \
    $$PWD/json_scanner.h

SOURCES += \
    $$PWD/serializerrunnable.cpp \
    $$PWD/serializer.cpp \
    $$PWD/qobjecthelper.cpp \
    $$PWD/parserrunnable.cpp \
    $$PWD/parser.cpp \
    $$PWD/json_scanner.cpp \
    $$PWD/json_parser.cc
