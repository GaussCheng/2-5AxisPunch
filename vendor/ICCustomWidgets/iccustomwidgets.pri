INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
include(iccustomwidgetsconfig.pri)
include(common/common.pri)

contains( UseWidgets, icframebutton ){
include(icframebutton/icframebutton.pri)
}

contains( UseWidgets, icdoublespinbox ){
include(icdoublespinbox/icdoublespinbox.pri)
}

contains( UseWidgets, icspinbox ){
include(icspinbox/icspinbox.pri)
}

contains( UseWidgets, iccheckbox ){
include(iccheckbox/iccheckbox.pri)
}

contains( UseWidgets, icunitlabel ){
include(icunitlabel/icunitlabel.pri)
}

contains( UseWidgets, icclocklabel ){
include(icclocklabel/icclocklabel.pri)
}

contains( UseWidgets, icusbstatus ){
include(icusbstatus/icusbstatus.pri)
}

contains( UseWidgets, iccombobox ){
include(iccombobox/iccombobox.pri)
}

contains( UseWidgets, icstatuslabel ){
include(icstatuslabel/icstatuslabel.pri)
}

contains( UseWidgets, icbarrel ){
include(icbarrel/icbarrel.pri)
}

contains( UseWidgets, icinjectionmachinewidget ){
include(icinjectionmachinewidget/icinjectionmachinewidget.pri)
}

contains( UseWidgets, ictwostatuslabel ){
include(ictwostatuslabel/ictwostatuslabel.pri)
}

contains( UseWidgets, icalarmbar ){
include(icalarmbar/icalarmbar.pri)
}

contains( UseWidgets, iclabel ){
include(iclabel/iclabel.pri)
}

contains( UseWidgets, icinputmethodedit ){
include(icinputmethodedit/icinputmethodedit.pri)
}

contains( UseWidgets, ictableview ){
include(ictableview/ictableview.pri)
}

contains( UseWidgets, icbutton){
include(icbutton/icbutton.pri)
}

contains( UseWidgets, iclineedit ){
include(iclineedit/iclineedit.pri)
}

contains( UseWidgets, icsplashscreen ){
include(icsplashscreen/icsplashscreen.pri)
}

contains( UseWidgets, icgroupbox ){
include(icgroupbox/icgroupbox.pri)
}

contains( UseWidgets, icrecordtablemodel ){
include(icrecordtablemodel/icrecordtablemodel.pri)
}

contains( UseWidgets, icupdatepackmodel) {
include(icupdatepackmodel/icupdatepackmodel.pri)
}

contains( UseWidgets, icalarmtablemodel) {
include(icalarmtablemodel/icalarmtablemodel.pri)
include(icalarmtabledelegate/icalarmtabledelegate.pri)
}

contains( UseWidgets, icslidingwidget) {
include(icslidingwidget/icslidingwidget.pri)
}

contains( UseWidgets, icplots) {
include(icplots/icplots.pri)
}

contains( UseWidgets, icpassworddialog) {
include(icpassworddialog/icpassworddialog.pri)
}

contains( UseWidgets, ictipswidget) {
include(ictipswidget/ictipswidget.pri)
}

contains( UseWidgets, ickeyboardbutton) {
include(ickeyboardbutton/ickeyboardbutton.pri)
}

contains( UseWidgets, icbindimage) {
include(icbindimage/icbindimage.pri)
}

contains(UseWidgets,iclistwidget){
include(iclistwidget/iclistwidget.pri)
}

contains(UseWidgets,ictablewidget){
include(ictablewidget/ictablewidget.pri)
}

#RESOURCES += customwidgetspics.qrc

#HEADERS +=

#SOURCES +=
