INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += icutility.cpp \
    #$$PWD/ictimerpool.cpp \
    $$PWD/icupdatesystem.cpp \
    $$PWD/icobjectpool.cpp \
    $$PWD/icflagspool.cpp \
    $$PWD/iccyclearray.cpp \
    $$PWD/icpic32uartbootloaderprotocol.cpp \
    $$PWD/icalarmclock.cpp \
    $$PWD/icregister.cpp \
    $$PWD/iccommonappsettings.cpp

HEADERS += $$PWD/icutility.h\
        $$PWD/ICUtility_global.h \
    #$$PWD/ictimerpool.h \
    $$PWD/containerinit.h \
    $$PWD/icoptimize.h \
    $$PWD/icupdatesystem.h \
    $$PWD/icobjectpool.h \
    $$PWD/icflagspool.h \
    $$PWD/iccyclearray.h \
    $$PWD/icpic32uartbootloaderprotocol.h \
    $$PWD/icalarmclock.h \
    $$PWD/icregister.h \
    $$PWD/iccommonappsettings.h

