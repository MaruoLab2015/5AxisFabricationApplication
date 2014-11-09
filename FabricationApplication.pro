#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T18:07:19
#
#-------------------------------------------------

QT       += core gui opengl serialport

CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FabricationApplication
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingDialog/stagesettingdialog.cpp \
    printpanel/stagewidget.cpp \
    printpanel/printpanel.cpp \
    printpanel/stagecontroller.cpp \
    model/gcode.cpp \
    model/responseanalyzer.cpp \
    printpanel/stage.cpp \
    printpanel/shutter.cpp \
    convert/convertpanel.cpp \
    editor/editorpanel.cpp \
    graphic/graphicswidget.cpp \
    graphic/renderer.cpp \
    graphic/scene.cpp \
    graphic/camera.cpp \
    graphic/light.cpp \
    graphic/geometry.cpp \
    graphic/mesh.cpp \
    graphic/face.cpp \
    graphic/vector3.cpp \
    graphic/material.cpp \
    graphicwidget.cpp

HEADERS  += mainwindow.h \
    settingDialog/stagesettingdialog.h \
    printpanel/stagewidget.h \
    printpanel/printpanel.h \
    enumList.h \
    printpanel/stagecontroller.h \
    model/gcode.h \
    model/responseanalyzer.h \
    printpanel/stage.h \
    printpanel/shutter.h \
    convert/convertpanel.h \
    editor/editorpanel.h \
    graphic/graphicswidget.h \
    graphic/GIThree.h \
    graphic/renderer.h \
    graphic/scene.h \
    graphic/camera.h \
    graphic/light.h \
    graphic/geometry.h \
    graphic/mesh.h \
    graphic/face.h \
    graphic/vector3.h \
    graphic/material.h \
    graphicwidget.h

FORMS    += mainwindow.ui \
    settingDialog/stagesettingdialog.ui \
    printpanel/printpanel.ui \
    convert/convertpanel.ui \
    editor/editorpanel.ui \
