TEMPLATE = subdirs
CONFIG += ordered
CUSTOM_LIBS = -ltcmalloc_minimal
#INCLUDEPATH += ../ICCustomWidgets/include
#LIBS += -lICCustomWidgets
SUBDIRS += ICUtility \
    #ICAlgorithm \
    ICCore \
    #ICPlugins \
    ICGUI
    #Tests/IndustrialSystemFrameworkUnitTests
system(python tools/CPHeader.py . include)
TRANSLATIONS += IndustrialSystemFramework_zh_CN.ts \
    IndustrialSystemFramework_en_US.ts
