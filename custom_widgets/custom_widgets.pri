INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include (icpagetag/icpagetag.pri)
include (systemstatusframe/systemstatusframe.pri)
include (icpagetab/icpagetab.pri)
include (iciomonitor/iciomonitor.pri)
include (icframe/icframe.pri)
#include (axisrunningstatus/axisrunningstatus.pri)
include (iccommunicationwidget/iccommunicationwidget.pri)
#include (iclabelwrapper/iclabelwrapper.pri)
include (lineeditwithvirtualnumerickeypad/lineeditwithvirtualnumerickeypad.pri)
include (lineeditwithvirtualkeyboard/lineeditwithvirtualkeyboard.pri)
include (cycletimeandfinished/cycletimeandfinished.pri)
include (programhead/programhead.pri)
#include (currentchildpagename/currentchildpagename.pri)
#include (icaxispositionlabelwrapper/icaxispositionlabelwrapper.pri)
include (ictwoselectioncomboboxwrapper/ictwoselectioncomboboxwrapper.pri)
include (iclineeditwrapper/iclineeditwrapper.pri)
include (iccommandkeywrapper/iccommandkeywrapper.pri)
include (iciopointwrapper/iciopointwrapper.pri)
include (icpictureview/icpictureview.pri)
include (iccheckedbutton/iccheckedbutton.pri)

contains(DEFINES, HC_SK_8){
    FORMS += custom_widgets/icinputmethodkeyboard.ui

}

contains(DEFINES, HC_SK_5){
    FORMS += custom_widgets/icinputmethodkeyboard_5.ui
}

FORMS += \
    custom_widgets/icpasswordmodifydialog.ui \
    custom_widgets/icwidgetitemkeyboard.ui



HEADERS += \
    custom_widgets/ictablewidget.h \
    custom_widgets/ictableview.h \
    custom_widgets/icpasswordmodifydialog.h \
    custom_widgets/icmessagebox.h \
    custom_widgets/iclistwidget.h \
    custom_widgets/iccombobox.h \
    custom_widgets/icgroupbox.h \
    custom_widgets/ictabwidget.h \
    custom_widgets/icinputmethodkeyboard.h \
    custom_widgets/icplaintextedit.h \
    custom_widgets/icbuttongroup.h\
    custom_widgets/icinputmethodedit.h \
    custom_widgets/icwidgetitemkeyboard.h  \
    custom_widgets/icmainframe.h

SOURCES += \
    custom_widgets/ictablewidget.cpp \
    custom_widgets/ictableview.cpp \
    custom_widgets/icpasswordmodifydialog.cpp \
    custom_widgets/icmessagebox.cpp \
    custom_widgets/iclistwidget.cpp \
    custom_widgets/iccombobox.cpp \
    custom_widgets/icgroupbox.cpp \
    custom_widgets/ictabwidget.cpp \
    custom_widgets/icinputmethodkeyboard.cpp \
    custom_widgets/icplaintextedit.cpp \
    custom_widgets/icbuttongroup.cpp \
    custom_widgets/icinputmethodedit.cpp \
    custom_widgets/icwidgetitemkeyboard.cpp  \
    custom_widgets/icmainframe.cpp
