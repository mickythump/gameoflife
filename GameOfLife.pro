#-------------------------------------------------
#
# Project created by QtCreator 2015-09-08T21:30:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameOfLife
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    game.cpp \
    patterns.cpp

HEADERS  += mainwindow.h \
    game.h \
    patterns.h

FORMS    += mainwindow.ui

RESOURCES += \
    ../../../../Pictures/png/icons.qrc
