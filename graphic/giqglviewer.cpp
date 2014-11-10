#include "giqglviewer.h"

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
//  help();
//  startAnimation();
}

void GIQGLViewer::draw()
{
    drawAxis();
}

void GIQGLViewer::animate()
{
//    qDebug() << "animate";
}
