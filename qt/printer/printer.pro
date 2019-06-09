#-------------------------------------------------
#
# Project created by QtCreator 2019-05-20T14:34:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = printer
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    iconhelper.cpp \
    keyBoard.cpp \
    inputdialog.cpp \
    wifimanager.cpp \
    printermanager.cpp

HEADERS  += dialog.h \
    iconhelper.h \
    keyBoard.h \
    inputdialog.h \
    wifimanager.h \
    printermanager.h

FORMS    += dialog.ui \
    inputdialog.ui

RESOURCES += \
    image.qrc \
    qss.qrc

CONFIG += c++11

INCLUDEPATH += ./include/

LIBS += -L./lib/ -lprinter
