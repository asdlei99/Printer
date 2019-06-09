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
    keyBoard.cpp

HEADERS  += dialog.h \
    iconhelper.h \
    keyBoard.h

FORMS    += dialog.ui

RESOURCES += \
    image.qrc \
    qss.qrc
