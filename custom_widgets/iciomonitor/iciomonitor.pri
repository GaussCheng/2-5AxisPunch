INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
HEADERS += iciomonitor.h \
            pagetabsbar.h \
            pageswitchbar.h \
            iciomonitorpagebase.h \
            iciopoint.h \
            iciopage.h

SOURCES += iciomonitor.cpp \
            pagetabsbar.cpp \
            pageswitchbar.cpp \
            iciomonitorpagebase.cpp \
            iciopoint.cpp \
            iciopage.cpp

FORMS += \
        pageswitchbar.ui

RESOURCES += \
    custom_widgets/iciomonitor/iciomonitor.qrc
