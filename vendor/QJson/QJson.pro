TARGET = qjson
TEMPLATE = lib

OBJECTS_DIR = temp
UI_DIR = temp
MOC_DIR = temp
RCC_DIR = temp
DESTDIR = libs
QMAKE_CXX = ccache $${QMAKE_CXX}
CONFIG(debug, debug|release) {
#    LIBS += -lprofiler
DESTDIR = libs_debug
OBJECTS_DIR = temp_d
UI_DIR = temp_d
MOC_DIR = temp_d
RCC_DIR = temp_d
}

HEADERS += \
    serializerrunnable.h \
    serializer.h \
    qobjecthelper.h \
    qjson_export.h \
    qjson_debug.h \
    position.hh \
    parserrunnable.h \
    parser_p.h \
    parser.h \
    json_scanner.h

SOURCES += \
    serializerrunnable.cpp \
    serializer.cpp \
    qobjecthelper.cpp \
    parserrunnable.cpp \
    parser.cpp \
    json_scanner.cpp \
    json_parser.cc


system(python tools/CPHeader.py . include/QJson)

OTHER_FILES +=
