#-------------------------------------------------
#
# Project created by QtCreator 2016-04-21T21:25:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += printsupport
QT += webenginewidgets
QT += multimediawidgets
TARGET = action-labeling-road-defects
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    chartviewer.cpp \
    mapviewer.cpp \
    dataviewer.cpp \
    videoviewer.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    chartviewer.h \
    mapviewer.h \
    dataviewer.h \
    videoviewer.h

FORMS    += mainwindow.ui

DISTFILES += \
    google_maps.html

RESOURCES += \
    resource.qrc
