QT = core gui
TARGET = QxTimeLineEditor
TEMPLATE = lib
DESTDIR = $$PWD/../bin
DEFINES += QXTIMELINEEDITOR_LIBRARY

INCLUDEPATH += ../include

HEADERS += \
    keyframeitem.h \
    ../include/qxtimelineeditor_global.h \
    ../include/qxtimelineeditor.h \
    ../include/QxTimeLineEditor

SOURCES += \
    qxtimelineeditor.cpp \
    keyframeitem.cpp

RESOURCES += \
    resource.qrc
