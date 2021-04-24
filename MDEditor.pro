TEMPLATE = app
TARGET = mdeditor

#CONFIG += dll
#CONFIG += c++11

#DEFINES += MDEDITOR_BUILD

QT += widgets \
    webenginewidgets

HEADERS =\
    mdeditor.h \
    htmlviewer.h \
    dict.h

SOURCES =\
    mdeditor.cpp \
    main.cpp \
    htmlviewer.cpp

FORMS += \
    mdeditor.ui

RESOURCES += \
    mdeditor.qrc

