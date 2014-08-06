INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/iciomonitor.h \
            $$PWD/pagetabsbar.h \
            $$PWD/pageswitchbar.h \
            $$PWD/iciomonitorpagebase.h \
            $$PWD/iciopoint.h \
            $$PWD/iciopage.h

SOURCES += $$PWD/iciomonitor.cpp \
            $$PWD/pagetabsbar.cpp \
            $$PWD/pageswitchbar.cpp \
            $$PWD/iciomonitorpagebase.cpp \
            $$PWD/iciopoint.cpp \
            $$PWD/iciopage.cpp

FORMS += \
        pageswitchbar.ui

RESOURCES += \
    custom_widgets/iciomonitor/iciomonitor.qrc
