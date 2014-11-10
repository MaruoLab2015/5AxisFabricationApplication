#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T18:07:19
#
#-------------------------------------------------

QT       += core gui opengl serialport xml

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
    graphicwidget.cpp \
    QGLViewer/VRender/BackFaceCullingOptimizer.cpp \
    QGLViewer/VRender/BSPSortMethod.cpp \
    QGLViewer/VRender/EPSExporter.cpp \
    QGLViewer/VRender/Exporter.cpp \
    QGLViewer/VRender/FIGExporter.cpp \
    QGLViewer/VRender/gpc.cpp \
    QGLViewer/VRender/NVector3.cpp \
    QGLViewer/VRender/ParserGL.cpp \
    QGLViewer/VRender/Primitive.cpp \
    QGLViewer/VRender/PrimitivePositioning.cpp \
    QGLViewer/VRender/TopologicalSortMethod.cpp \
    QGLViewer/VRender/Vector2.cpp \
    QGLViewer/VRender/Vector3.cpp \
    QGLViewer/VRender/VisibilityOptimizer.cpp \
    QGLViewer/VRender/VRender.cpp \
    QGLViewer/camera.cpp \
    QGLViewer/constraint.cpp \
    QGLViewer/frame.cpp \
    QGLViewer/keyFrameInterpolator.cpp \
    QGLViewer/manipulatedCameraFrame.cpp \
    QGLViewer/manipulatedFrame.cpp \
    QGLViewer/mouseGrabber.cpp \
    QGLViewer/qglviewer.cpp \
    QGLViewer/quaternion.cpp \
    QGLViewer/saveSnapshot.cpp \
    QGLViewer/vec.cpp \
    graphic/giqglviewer.cpp

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
    graphicwidget.h \
    QGLViewer/VRender/AxisAlignedBox.h \
    QGLViewer/VRender/Exporter.h \
    QGLViewer/VRender/gpc.h \
    QGLViewer/VRender/NVector3.h \
    QGLViewer/VRender/Optimizer.h \
    QGLViewer/VRender/ParserGL.h \
    QGLViewer/VRender/Primitive.h \
    QGLViewer/VRender/PrimitivePositioning.h \
    QGLViewer/VRender/SortMethod.h \
    QGLViewer/VRender/Types.h \
    QGLViewer/VRender/Vector2.h \
    QGLViewer/VRender/Vector3.h \
    QGLViewer/VRender/VRender.h \
    QGLViewer/camera.h \
    QGLViewer/config.h \
    QGLViewer/constraint.h \
    QGLViewer/domUtils.h \
    QGLViewer/frame.h \
    QGLViewer/keyFrameInterpolator.h \
    QGLViewer/manipulatedCameraFrame.h \
    QGLViewer/manipulatedFrame.h \
    QGLViewer/mouseGrabber.h \
    QGLViewer/qglviewer.h \
    QGLViewer/quaternion.h \
    QGLViewer/vec.h \
    EnumList.h \
    graphic/giqglviewer.h

FORMS    += mainwindow.ui \
    settingDialog/stagesettingdialog.ui \
    printpanel/printpanel.ui \
    convert/convertpanel.ui \
    editor/editorpanel.ui \
    QGLViewer/VRenderInterface.ui \
    QGLViewer/ImageInterface.ui

OTHER_FILES += \
    QGLViewer/qglviewer-icon.xpm
