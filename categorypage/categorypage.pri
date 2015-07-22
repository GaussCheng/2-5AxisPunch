INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    categorypage/icinitialframe.h \
    categorypage/icalarmframe.h \
    categorypage/icmodifyframe.h \
    categorypage/icmonitorpageframe.h \
    categorypage/icsettingsframe.h  \
    icconfigstring.h

SOURCES += \
    categorypage/icinitialframe.cpp \
    categorypage/icalarmframe.cpp \
    categorypage/icmodifyframe.cpp \
    categorypage/icmonitorpageframe.cpp \
    categorypage/icsettingsframe.cpp    \
    icconfigstring.cpp

FORMS += \
    categorypage/icinitialframe.ui \
    categorypage/icalarmframe.ui \
    categorypage/icmodifyframe.ui \
    categorypage/icmonitorpageframe.ui \
    categorypage/icsettingsframe.ui

RESOURCES += \
    categorypage/resources.qrc
