#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T18:07:19
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FabricationApplication
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stagesettingdialog.cpp \
    masterthread.cpp

HEADERS  += mainwindow.h \
    stagesettingdialog.h \
    masterthread.h

FORMS    += mainwindow.ui \
    stagesettingdialog.ui
