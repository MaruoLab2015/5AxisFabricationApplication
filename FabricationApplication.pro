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
    stagewidget.cpp \
    stagethread.cpp \
    printpanel.cpp \
    stagecontroller.cpp \
    gcode.cpp \
    gcodeanalyzer.cpp

HEADERS  += mainwindow.h \
    stagesettingdialog.h \
    stagewidget.h \
    stagethread.h \
    printpanel.h \
    enumList.h \
    stagecontroller.h \
    gcode.h \
    gcodeanalyzer.h

FORMS    += mainwindow.ui \
    stagesettingdialog.ui \
    printpanel.ui
