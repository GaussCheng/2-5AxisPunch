INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    statuslabel.h \
    icsystemstatusframe.h \
    speedstatuslabel.h \
    operatingratiosetdialog.h \
    custom_widgets/systemstatusframe/icclosemoldlabel.h \
    custom_widgets/systemstatusframe/ictunemolddialog.h

SOURCES += \
    statuslabel.cpp \
    icsystemstatusframe.cpp \
    speedstatuslabel.cpp \
    operatingratiosetdialog.cpp \
    custom_widgets/systemstatusframe/icclosemoldlabel.cpp \
    custom_widgets/systemstatusframe/ictunemolddialog.cpp

FORMS += \
    icsystemstatusframe.ui \
    operatingratiosetdialog.ui \
    custom_widgets/systemstatusframe/ictunemolddialog.ui
