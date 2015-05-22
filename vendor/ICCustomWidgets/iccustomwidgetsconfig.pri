INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UseWidgets += icalarmbar
UseWidgets += icbarrel
UseWidgets += icbutton
UseWidgets += iccheckbox
UseWidgets += icclocklabel
UseWidgets += iccombobox
UseWidgets += icframebutton
UseWidgets += icgroupbox
UseWidgets += icinjectionmachinewidget
UseWidgets += iclabel
UseWidgets += iclineedit
contains(QMAKE_CC, gcc){
UseWidgets += icspinbox
UseWidgets += icdoublespinbox
}
UseWidgets += icsplashscreen
UseWidgets += icstatuslabel
UseWidgets += ictableview
UseWidgets += ictwostatuslabel
UseWidgets += icunitlabel
UseWidgets += icusbstatus
UseWidgets += icinputmethodedit
UseWidgets += icrecordtablemodel
UseWidgets += icupdatepackmodel
UseWidgets += icalarmtablemodel
UseWidgets += icslidingwidget
#UseWidgets += icplots
UseWidgets += icpassworddialog
UseWidgets += ictipswidget
UseWidgets += ickeyboardbutton
UseWidgets += icbindimage
#UseWidgets += iclistwidget
#UseWidgets += ictablewidget


