#include "giqglviewer.h"
#include "QGLViewer/vec.h"

GIQGLViewer::GIQGLViewer(QWidget *parent) :
    QGLViewer(parent)
{
}

void GIQGLViewer::init()
{
//    qDebug() << "init";
  restoreStateFromFile();
  glDisable(GL_LIGHTING);
  glPointSize(3.0);
  setGridIsDrawn();

  initCamera();
//  help();
//  startAnimation();
}

void GIQGLViewer::initCamera()
{
    qglviewer::Camera *initCamera = camera();
    qglviewer::Vec cameraPos = qglviewer::Vec(2, 2, 2);
    initCamera->setPosition(cameraPos);
    initCamera->setUpVector(qglviewer::Vec(0,0,1));
    initCamera->lookAt(qglviewer::Vec());
    setCamera(initCamera);
}

void GIQGLViewer::draw()
{
    drawAxis();
}

void GIQGLViewer::animate()
{
//    qDebug() << "animate";
}
